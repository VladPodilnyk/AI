/**
 * file: randGen.hpp
 * synopsis: Implementation of random 
 *           generator.
 * author: Vladyslav Podilnyk
 */

#ifndef __AI_RANDGEN_HPP__
#define __AI_RANDGEN_HPP__

#include <random>
#include <utility>
#include <vector>
#include <algorithm>
#include "utils.hpp"

using ai::utils::value_t;

namespace ai::rgen {

template <typename T = value_t, 
          typename Distribution = std::uniform_real_distribution<T>,
          typename CustomGen = std::mt19937_64>
class RandGen
{
    public:
        RandGen(std::pair<T, T> limits)
            : dist{limits.first, limits.second} {};

        auto generate(size_t size) {
            std::random_device seed;
            CustomGen randNumberGen(seed());
            auto container = std::valarray<T>(size);
            std::generate(begin(container), end(container),
                          [&]() { return dist(randNumberGen); });
            return container;
        }

        auto randValue() {
            std::random_device seed;
            CustomGen randNumberGen(seed());
            return dist(randNumberGen);
        }

    private:
        Distribution dist;
};

} // rgen

#endif // __AI_RANDGEN_HPP__