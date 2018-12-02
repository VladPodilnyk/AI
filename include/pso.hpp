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

#include "randGen.hpp"

namespace ai {

/**
 * This is the constants for PSO algorithms, crCoef stands for cognitive force
 * coefficient and sfCoef stands for social force coefficient. These values 
 * define a direction in which a particle moves. For now, they are assigned 
 * to 2.0 which is perfect for most cases, but maybe in future we want to 
 * adjust default particle behaviour, so that's why it's better
 * to have constant fo that then hardcode them in PSO constructor.
 */
constexpr auto crCoef = 2.0; 
constexpr auto sfCoef = 2.0;

struct Particle
{
    std::valarray<double> currentPosition;
    std::valarray<double> velocity;
    std::valarray<double> personalBestPos;
    double personalBest;
    double velocityChangeRate;
};

class Function
{
    using FuncArguments = std::valarray<double>;
    using FuncType = std::function<double(FuncArguments&)>;
    using FuncLimits = std::pair<double, double>;

    public:
        Function() = default;
        explicit Function(FuncType fn, size_t dim, FuncLimits funcLimits)
            : func{fn}, dimensions{dim}, limits{funcLimits} {};

        double operator()(FuncArguments& args) { return func(args); };
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
        std::pair<double, std::valarray<double>> operator()();
        ~Pso() = default;

    private:
        // functions
        inline void initParticlePos();
        inline void initVelocity();
        inline void updatePosition(Particle& particle);
        inline void updateVelocity(Particle& particle, std::pair<double, double> r);
        inline void updatePersonalBest();
        inline void updateGlobalBest();
        void updateParticle(std::pair<double, double> r);
        void convergenceStep();
        bool isConverged();

        // data
        using Swarm = std::array<Particle, swarmSize>;
        Swarm swarmColony;
        Function fn;
        double gBest;
        std::valarray<double> gBestPos;
        double cognitiveForceCoef;
        double socialForceCoef;
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
    //auto randNumberGen = rgen::RandGen<>(std::make_pair(0, 50));
    auto dimensions = fn.getDimensions();
    for (auto& particle : swarmColony) {
        particle.velocity.resize(dimensions);
        std::fill(begin(particle.velocity), end(particle.velocity), 0);
        //particle.velocity = randNumberGen.generate(dimensions);
        particle.velocityChangeRate = 0;
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::updatePosition(Particle& particle)
{
    particle.currentPosition += particle.velocity;
}

template <size_t swarmSize>
void Pso<swarmSize>::updateVelocity(Particle& particle, std::pair<double, double> r) 
{
    std::valarray<double> cognitiveForce =
        cognitiveForceCoef * r.first * (particle.personalBestPos - particle.currentPosition);

    std::valarray<double> socialForce =
        socialForceCoef * r.second * (gBestPos - particle.currentPosition);

    particle.velocity *= 0.72984; // MAKE DYNAMIC INARTIA WEIGHT
    particle.velocity += cognitiveForce + socialForce;
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
void Pso<swarmSize>::updateParticle(std::pair<double, double> r)
{
    for (auto& particle : swarmColony) {
        updateVelocity(particle, r);
        updatePosition(particle);
    }
}

template <size_t swarmSize>
void Pso<swarmSize>::convergenceStep()
{
    auto randGen = rgen::RandGen<>(std::make_pair(0.0, 1.0));
    auto rCoefs = randGen.genRandPair();

    updateParticle(rCoefs);
    updatePersonalBest();
    updateGlobalBest();
}

template <size_t swarmSize>
bool Pso<swarmSize>::isConverged()
{
// Nothing
}

template <size_t swarmSize>
Pso<swarmSize>::Pso(Function& f, double cognitiveForceCoef, double socialForceCoef)
{
    this->cognitiveForceCoef = cognitiveForceCoef;
    this->socialForceCoef = socialForceCoef;
    fn = f;

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
std::pair<double, std::valarray<double>> Pso<swarmSize>::operator()()
{
//    while (isConverged()) {
    for (size_t i = 0; i < 100; ++i) {
        convergenceStep();
        std::cout << "BEST[" << i << "] = " << gBest << std::endl;
    }
//    }
    return std::make_pair(gBest, gBestPos);
}

} // ai

#endif // __AI_PSO_HPP__