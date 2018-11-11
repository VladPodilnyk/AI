/**
 * file: utils.hpp
 * synopsis: A set of predefined funtions
 *           for pso
 * author: Vladyslav Podilnyk
 */

#ifndef __AI_UTILS_HPP__
#define __AI_UTILS_HPP__

#include <cmath>
#include <vector>
#include <array>
#include <valarray>
#include <numeric>

namespace ai::utils {

constexpr auto e = 2.71828182845904523536;
constexpr auto pi = 3.14159265358979323846;

double spherefn(std::valarray<double>& args) {
    /*auto sumOfSqueres = [&args](double first, double second) {
        return first + second * second;
    };*/
    auto sum = 0.0;
    for (auto& value : args) {
        sum += value * value;
    }

    return sum;//std::accumulate(begin(args), end(args), 0, sumOfSqueres);
}

/*
double ackleyfn(std::vector<double>& args) {
    auto alpha = 20;
    auto result = -alpha * std::exp(-0.2 * std::sqrt(spherefn(args) / args.size()));
    auto sumOfCos = [&args](double first, double second) {
        return first + std::cos(2 * pi * second);
    };

    auto sum = std::accumulate(begin(args), end(args), 0, sumOfCos);
    return result - std::exp(sum / args.size()) + alpha + pi;
}

double griewankfn(std::vector<double>& args) {
    auto index = 1;
    auto productOfCos = [&args, &index](double first, double second) {
        auto result  = first * std::cos(second / index);
        ++index;
        return result;
    };

    auto product = std::accumulate(begin(args), end(args), 1, productOfCos);
    return spherefn(args) / 4000 - product + 1;
}

double rastriginfn(std::vector<double>& args) {
    // TODO
}

double rosenbrokfn(std::vector<double>& args) {
    // TODO
}
*/
} // utils

#endif // __AI_UTILS_HPP__