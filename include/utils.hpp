/**
 * file: utils.hpp
 * synopsis: A set of predefined funtions
 *           for pso
 * author: Vladyslav Podilnyk
 */

#ifndef __AI_UTILS_HPP__
#define __AI_UTILS_HPP__

#include <cmath>
#include <valarray>
#include <array>
#include <valarray>
#include <numeric>
#include <string>

namespace ai::utils {

using value_t = long double;

constexpr auto e = 2.71828182845904523536;
constexpr auto pi = 3.14159265358979323846;

enum class FuncType {Sphere, Ackley, Griewank, Rastrigin, Rosenbrok};

value_t spherefn(std::valarray<value_t>& args) {
    auto sumOfSqueres = [&args](value_t first, value_t second) {
        return first + second * second;
    };

    return std::accumulate(begin(args), end(args), 0.0, sumOfSqueres);
}

value_t ackleyfn(std::valarray<value_t>& args) {
    auto alpha = 20;
    auto result = -alpha * std::exp(-0.2 * std::sqrt(spherefn(args) / args.size()));
    auto sumOfCos = [&args](value_t first, value_t second) {
        return first + std::cos(2 * pi * second);
    };

    auto sum = std::accumulate(begin(args), end(args), 0.0, sumOfCos);
    return result - std::exp(sum / args.size()) + alpha + pi;
}

value_t griewankfn(std::valarray<value_t>& args) {
    auto index = 1;
    auto productOfCos = [&args, &index](value_t first, value_t second) {
        auto result  = first * std::cos(second / index);
        ++index;
        return result;
    };

    auto product = std::accumulate(begin(args), end(args), 1.0, productOfCos);
    return spherefn(args) / 4000 - product + 1;
}

value_t rastriginfn(std::valarray<value_t>& args) {
    auto result = value_t{10} * args.size();
    auto sum = [&args](value_t first, value_t second) {
        return first + (second * second - 10.0 * cos(2.0 * pi * second));
    };

    return result + std::accumulate(begin(args), end(args), 0.0, sum);
}

value_t rosenbrokfn(std::valarray<value_t>& args) {
    auto resultVect = std::valarray<value_t>(args.size() - 1);
    auto newVal = [&args](value_t first, value_t second) {
        return 100.0 * (second - first * first) * (second -first * first)
                + (first - 1) * (first - 1);
    };

    std::transform(begin(args), end(args) - 1, std::next(begin(args)),
                    begin(resultVect), newVal);
    return resultVect.sum();
}

void prettyPrint(value_t min, std::valarray<value_t>& coordinates, FuncType type) {
    auto testName = std::string{};
    switch (type) {
        case FuncType::Sphere: testName = "TEST FOR A SPERE FUNCTION"; break;
        case FuncType::Ackley: testName = "TEST FOR A ACKLEY FUNCTION"; break;
        case FuncType::Griewank: testName = "TEST FOR A GRIENWANK FUNCTION"; break;
        case FuncType::Rastrigin: testName = "TEST FOR A RASTRIGIN FUNCTION"; break;
        case FuncType::Rosenbrok: testName = "TEST FOR A ROSENBROK FUNCTION"; break;
    }

    std::cout << "\n" << testName << "\n";
    std::cout << "Min = "<< min << "\n";
    std::cout << "Best position = [";

    auto lastValue = coordinates[coordinates.size() - 1];
    for (const auto& value : coordinates) {
        if (value != lastValue) {
            std::cout << value << ", ";
        } else {
            std::cout << value;
        }
    }
    std::cout << "]\n\n";
}

template <typename T>
void print(T container) {
    std::cout << "[ ";
    for (const auto& item : container) {
        std::cout << item << ", ";
    }
    std::cout << "]\n";
}

} // utils

#endif // __AI_UTILS_HPP__