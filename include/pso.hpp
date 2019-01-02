/**
 * file: pso.hpp
 * synopsis: Implemenation for the PSO algorithm
 * author: Vladyslav Podilnyk
 */

#ifndef __AI_PSO_HPP__
#define __AI_PSO_HPP__

#include <cmath>
#include <array>
#include <algorithm>
#include <functional>
#include <limits>
#include <valarray>

#include "utils.hpp"
#include "randGen.hpp"

#define PRINT_BEST 1
#define RETURN_TO_BOUND 0
#define CLAMP_VELOCITY 0
#define DYNAMIC_INERTIA_WEIGHT 0 // poor implementation, DO NOT USE THIS
#define CALCULATE_AVERAGE_VELOCITY 0 // stop criteria

using ai::utils::value_t;
using ai::utils::print; // for Debug purposes

namespace ai {

/**
 * This is the constants for PSO algorithms, crCoef stands for cognitive force
 * coefficient and sfCoef stands for social force coefficient. These values 
 * define a direction in which a particle moves.
 */
constexpr auto crCoef = 2.0;
constexpr auto sfCoef = 2.0;

constexpr auto inrWeightMax = 0.92984;
constexpr auto inrWeightMin = 0.42984;
constexpr auto inrWeight = 0.72984;
constexpr auto eps = value_t {1e-160};
constexpr auto lastIterNumber = size_t{1000};

struct Particle
{
    std::valarray<value_t> currentPosition;
    std::valarray<value_t> velocity;
    std::valarray<value_t> personalBestPos;
    value_t personalBest;

#if CALCULATE_AVERAGE_VELOCITY
    value_t averageVelocity;
#endif
};

class Function
{
    using FuncArguments = std::valarray<value_t>;
    using FuncType = std::function<value_t(FuncArguments&)>;
    using FuncLimits = std::pair<double, double>;

    public:
        Function() = default;
        explicit Function(FuncType fn, size_t dim, FuncLimits funcLimits)
            : func{fn}, dimensions{dim}, limits{funcLimits} {};

        value_t operator()(FuncArguments& args) { return func(args); };
        FuncLimits getFuncLimits() { return limits; };
        size_t getDimensions() { return dimensions; };
        ~Function() = default;

    private:
        FuncType func;
        size_t dimensions;
        FuncLimits limits;
};

template <size_t swarmSize>
class Pso
{
    public:
        Pso() = default;
#if CALCULATE_AVERAGE_VELOCITY
        Pso(Function& f, double cognitiveForceCoef, double socialForceCoef,
            double inertiaWeight, value_t eps);
        Pso(Function& f) : Pso(f, crCoef, sfCoef, inrWeight, eps) {};
#else
        Pso(Function& f, double cognitiveForceCoef, double socialForceCoef,
            double inertiaWeight);
        Pso(Function& f) : Pso(f, crCoef, sfCoef, inrWeight) {};
#endif
        std::pair<value_t, std::valarray<value_t>> operator()();
        ~Pso() = default;

    private:
        // functions
        inline void initParticlePos();
        inline void initVelocity();
        inline void updatePosition(Particle& particle);
        inline void updateVelocity(Particle& particle);
        inline void updatePersonalBest();
        inline void updateGlobalBest();
        void clampVelocities(Particle& particle);
        void retParticleToBound(Particle& particle);
        void updateParticle();
        void convergenceStep();
        bool isConverged();

        // data
        using Swarm = std::array<Particle, swarmSize>;
        Swarm swarmColony;
        Function fn;
        value_t gBest;
        value_t maxVelocity;
        std::valarray<value_t> gBestPos;
        double cognitiveForceCoef;
        double socialForceCoef;
        double inertiaWeight;
        value_t eps;
        size_t stopCounter = lastIterNumber;
        size_t step; // for dynamic calculation of inertia weight
        bool isStuckOrConverged = false;
        bool maybeStuck = false;
};

template <size_t swarmSize>
void Pso<swarmSize>::initParticlePos()
{
    auto randNumberGen = rgen::RandGen<>(fn.getFuncLimits());
    auto dimensions = fn.getDimensions();
    for (auto& particle : swarmColony) {
        particle.currentPosition = randNumberGen.generate(dimensions);
        particle.personalBestPos = particle.currentPosition;
        particle.personalBest = fn(particle.personalBestPos);
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::initVelocity()
{
    auto dimensions = fn.getDimensions();
    for (auto& particle : swarmColony) {
        particle.velocity.resize(dimensions);
        std::fill(begin(particle.velocity), end(particle.velocity), 0);
#if CALCULATE_AVERAGE_VELOCITY
        particle.averageVelocity = 0;
#endif
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::retParticleToBound(Particle& particle)
{
    for (auto& value : particle.currentPosition) {
        if (value > fn.getFuncLimits().second) {
            value = fn.getFuncLimits().second / 2;

        } else if (value < fn.getFuncLimits().first) {
            value = fn.getFuncLimits().first / 2;
        }
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::updatePosition(Particle& particle)
{
    particle.currentPosition += particle.velocity;
#if RETURN_TO_BOUND
    retParticleToBound(particle);
#endif
}

template <size_t swarmSize>
void Pso<swarmSize>::clampVelocities(Particle& particle)
{
    for (auto& value : particle.velocity) {
        if (value > maxVelocity) {
            value = maxVelocity;
        }
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::updateVelocity(Particle& particle)
{
    auto randGen = rgen::RandGen<>(std::make_pair(0.0, 1.0));
    auto rfirst = randGen.generate(particle.velocity.size());
    auto rsecond = randGen.generate(particle.velocity.size());

    std::valarray<value_t> cognitiveForce =
        rfirst * (particle.personalBestPos - particle.currentPosition);
    cognitiveForce *= cognitiveForceCoef;

    std::valarray<value_t> socialForce =
        rsecond * (gBestPos - particle.currentPosition);
    socialForce *= socialForceCoef;

    /**
     * TODO: Implement calculation for inertia weight
     *       which should decrease from 0.9 to 0.4.
     *       Current implementation is bad. DO NOT USE IT !!!.
     */
#if DYNAMIC_INERTIA_WEIGHT
    particle.velocity *= inrWeightMax - (inrWeightMax - inrWeightMin)
                         * static_cast<double>(step) / lastIterNumber;
#else
    particle.velocity *= inertiaWeight;
#endif

    particle.velocity += cognitiveForce + socialForce;

#if CLAMP_VELOCITY
    clampVelocities(particle);
#endif

#if CALCULATE_AVERAGE_VELOCITY
    particle.averageVelocity = particle.velocity.sum() / particle.velocity.size();
#endif
}

template <size_t swarmSize>
void Pso<swarmSize>::updatePersonalBest()
{
    for (auto& particle : swarmColony) {
        auto res = fn(particle.currentPosition);
        if (res < particle.personalBest) {
            particle.personalBest = res;
            particle.personalBestPos = particle.currentPosition;
        }
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::updateGlobalBest()
{
    auto isGbestChanged = false;
    auto& bestParticle = swarmColony[0];

    for (auto& particle : swarmColony) {
        if (particle.personalBest < gBest) {
            gBest = particle.personalBest;
            bestParticle = particle;
            isGbestChanged = true;
        }
    }

    /**
     * TODO: Think about another stop criteria
     *       or improve an approach based on checking
     *       average swarm velocities.
     *
     *       This ugly workaround MUST BE removed !!!
     */
    if (isGbestChanged) {
        gBestPos = bestParticle.personalBestPos;
        maybeStuck = false;
    } else {
        maybeStuck = true;
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::updateParticle()
{
    for (auto& particle : swarmColony) {
        updateVelocity(particle);
        updatePosition(particle);
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::convergenceStep()
{
    updateParticle();
    updatePersonalBest();
    updateGlobalBest();

#if DYNAMIC_INERTIA_WEIGHT
    ++step;
#endif
}

template <size_t swarmSize>
bool Pso<swarmSize>::isConverged()
{
#if CALCULATE_AVERAGE_VELOCITY
    auto sum = value_t{0.0};
    for (const auto& particle : swarmColony) {
        sum += particle.averageVelocity;
    }

    auto averageSwarmVelocity = sum / swarmColony.size();

    if ((averageSwarmVelocity < eps) && isStuckOrConverged) {
        stopCounter--;
    } else if ((averageSwarmVelocity < eps)) {
        isStuckOrConverged = true;
    } else {
        isStuckOrConverged = false;
        stopCounter = lastIterNumber;
    }
#else
    if (maybeStuck) {
        stopCounter--;
    } else {
        stopCounter = lastIterNumber;
    }
#endif

    if (stopCounter) {
        return false;
    }
    return true;
}

#if CALCULATE_AVERAGE_VELOCITY
template <size_t swarmSize>
Pso<swarmSize>::Pso(Function& f, double cognitiveForceCoef, double socialForceCoef,
                    double inertiaWeight, value_t eps)
#else
template <size_t swarmSize>
Pso<swarmSize>::Pso(Function& f, double cognitiveForceCoef, double socialForceCoef,
                    double inertiaWeight)
#endif
{
    this->cognitiveForceCoef = cognitiveForceCoef;
    this->socialForceCoef = socialForceCoef;
    this->inertiaWeight = inertiaWeight;
#if CALCULATE_AVERAGE_VELOCITY
    this->eps = eps;
#endif
    fn = f;

    auto limits = fn.getFuncLimits();
    maxVelocity = 0.5 * (limits.second - limits.first);
    step = 0;

    initParticlePos();
    initVelocity();

    auto min = std::numeric_limits<double>::max();
    auto& bestParticle = swarmColony[0];
    for (auto& particle : swarmColony) {
        if (particle.personalBest < min) {
            min = particle.personalBest;
            bestParticle = particle;
        }
    }

    gBest = bestParticle.personalBest;
    gBestPos = bestParticle.personalBestPos;
}

template <size_t swarmSize>
std::pair<value_t, std::valarray<value_t>> Pso<swarmSize>::operator()()
{
    while (!isConverged()) {
        convergenceStep();
#if PRINT_BEST
        std::cout << "(DEBUG PRINT) Best = " << gBest << std::endl;
#endif
    }
    return std::make_pair(gBest, gBestPos);
}

} // ai

#endif // __AI_PSO_HPP__