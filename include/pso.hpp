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

#include <iostream> // DEBUG

#include "utils.hpp"
#include "randGen.hpp"

using ai::utils::value_t;

namespace ai {

/**
 * This is the constants for PSO algorithms, crCoef stands for cognitive force
 * coefficient and sfCoef stands for social force coefficient. These values 
 * define a direction in which a particle moves.
 */
constexpr auto crCoef = 2.0;
constexpr auto sfCoef = 2.0;

template <typename T>
void print(T container) {
    std::cout << "[ ";
    for (const auto& item : container) {
        std::cout << item << ", ";
    }
    std::cout << "]\n";
}


struct Particle
{
    std::valarray<value_t> currentPosition;
    std::valarray<value_t> velocity;
    std::valarray<value_t> personalBestPos;
    value_t personalBest;
    value_t averageVelocity;
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
        Pso(Function& f, double cognitiveForceCoef, double socialForceCoef);
        Pso(Function& f) : Pso(f, crCoef, sfCoef) {};
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
        void updateParticle();
        void convergenceStep();
        void clampVelocity(std::valarray<value_t>& pVelocities);
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
        size_t stopCounter;
        constexpr auto lastIterNumber = size_t{1000};
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
        particle.averageVelocity = 0;
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::updatePosition(Particle& particle)
{
    particle.currentPosition += particle.velocity;
    //print(particle.currentPosition);
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

    particle.velocity *= 0.42984; // MAKE DYNAMIC INERTIA WEIGHT
    particle.velocity += cognitiveForce + socialForce;

    particle.averageVelocity = particle.velocity.sum() / particle.velocity.size();
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
    //auto& bestParticle = swarmColony[0];
    for (auto& particle : swarmColony) {
        if (particle.personalBest < gBest) {
            gBest = particle.personalBest;
            gBestPos = particle.personalBestPos;
            //bestParticle = particle;
        }
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
}

template <size_t swarmSize>
void Pso<swarmSize>::clampVelocity(std::valarray<value_t>& pVelocities) // Consider to remove
{
    for (auto& value : pVelocities) {
        if (value > maxVelocity) {
            value = maxVelocity;
        }
    }
}

template <size_t swarmSize>
bool Pso<swarmSize>::isConverged()
{

    return false;
}

template <size_t swarmSize>
Pso<swarmSize>::Pso(Function& f, double cognitiveForceCoef, double socialForceCoef)
{
    this->cognitiveForceCoef = cognitiveForceCoef;
    this->socialForceCoef = socialForceCoef;
    fn = f;

    auto limits = fn.getFuncLimits();
    maxVelocity = 0.5 * (limits.second - limits.first);

    std::cout << "maxVel = "<< maxVelocity << "\n";

    initParticlePos();
    initVelocity();

    auto min = std::numeric_limits<double>::max();
    auto bestParticle = &swarmColony[0];
    for (auto& particle : swarmColony) {
        if (particle.personalBest < min) {
            min = particle.personalBest;
            bestParticle = &particle;
        }
    }

    gBest = bestParticle->personalBest;
    gBestPos = bestParticle->personalBestPos;
}

template <size_t swarmSize>
std::pair<value_t, std::valarray<value_t>> Pso<swarmSize>::operator()()
{
//    while (isConverged()) {
    for (size_t i = 0; i < 100000; ++i) {
        convergenceStep();
        std::cout << "BEST[" << i << "] = " << gBest << std::endl;
    }
//    }
    return std::make_pair(gBest, gBestPos);
}

} // ai

#endif // __AI_PSO_HPP__