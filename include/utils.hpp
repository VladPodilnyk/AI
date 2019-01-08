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
#include <vector>
#include <numeric>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>

namespace ai::utils {

using value_t = long double;

template <typename T>
using matrix = std::vector<std::vector<T>>;

using path = std::vector<size_t>;

constexpr auto e = 2.71828182845904523536;
constexpr auto pi = 3.14159265358979323846;

enum class FuncType {Sphere, Ackley, Griewank, Rastrigin, Rosenbrok};

value_t spherefn(std::valarray<value_t>& args);

value_t ackleyfn(std::valarray<value_t>& args);

value_t griewankfn(std::valarray<value_t>& args);

value_t rastriginfn(std::valarray<value_t>& args);

value_t rosenbrokfn(std::valarray<value_t>& args);

template <typename T>
void print(T container) {
    if (!container.size()) {
        std::cout << "[]\n";
        return;
    }

    std::cout << "[ ";
    auto lastValue = end(container) - 1;

    for (auto value = begin(container); value != end(container); ++value) {
        if (value != lastValue) {
            std::cout << *value << ", ";
        } else {
            std::cout << *value;
        }
    }
    std::cout << "]\n\n";
}

void prettyPrint(value_t min, std::valarray<value_t>& coordinates, FuncType type);

/**
 * TODO: make parser safe
 *       to use.
 */
class Parser
{
    public:
        static matrix<size_t> getGraphFromFile(std::string filename)
        {
            std::ifstream inputData(filename);
            std::istringstream tokenStream(filename);

            auto split = [](auto line) {
                std::istringstream iss(line);
                std::vector<std::string> tokens = {std::istream_iterator<std::string> {iss},
                                                    std::istream_iterator<std::string> {}};
                return tokens;
            };

            auto graph = matrix<size_t>();
            auto line = std::string{};
            auto lineNumber = size_t{0};

            if (inputData.is_open()) {
                while (std::getline(inputData, line)) {
                    if (line[0] == 'p') {
                        auto tokens = split(line);
                        auto graphSize = std::stoul(tokens[1]);

                        for (size_t index = 0; index < graphSize; ++index) {
                            graph.emplace_back(path(graphSize));
                        }
                    } else if (line[0] == 'i') {
                        auto tokens = split(line);
                        for (size_t column = 0; column < graph.size(); ++column) {
                            graph[lineNumber][column] = std::stoul(tokens[column + 1]);
                        }
                        ++lineNumber;
                    }
                }
            } else {
                throw std::ios_base::failure("No file with given name.");
            }

            inputData.close();
            return graph;
        };
};

} // utils

#endif // __AI_UTILS_HPP__
