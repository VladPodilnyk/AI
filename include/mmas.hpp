/**
 * file: mmas.hpp
 * synopsis: Implemenation for the MIN MAX Ant System algorithm
 * author: Vladyslav Podilnyk
 */

#ifndef __AI_MMAS_HPP__
#define __AI_MMAS_HPP__

#include <algorithm>
#include "utils.hpp"

namespace ai {

class Graph
{
    public:
        Graph() = default;
        explicit Graph(const utils::matrix& wages) { paths = wages; };
        size_t getPathWeight(const std::vector<size_t>& path)
        {
            auto result = size_t{0};
            for (size_t node = 0; node < path.size() - 1; ++node) {
                result = paths[node][node + 1];
            }
            return result;
        };

        size_t getWeight(size_t startNode, size_t endNode)
        {
            paths[startNode][endNode]; 
        };

        size_t size() { return paths.size(); };
        ~Graph() = default;
    private:
        utils::matrix paths;
};

struct AntSystemConfig
{
    double alpha = 0.0;
    double beta = 0.0;
    double initPheromoneLevel = 0.0;
};

template <size_t colonySize>
class Ants
{
    public:
        Ants() = default;
        Ants(const Graph& graph, AntSystemConfig config = AntSystemConfig{});
        std::vector<size_t> operator()(size_t startPoint, size_t endPoint);
        ~Ants() = default;
    private:
        Graph graph;
        AntsSystemConfig config;
        utils::matrix pheromones;
        std::array<size_t, colonySize> antsColony;
        std::vector<size_t> shortestPath;
};

template <size_t colonySize>
Ants<colonySize>::Ants(const Graph& graph, AntSystemConfig config)
{
    this->graph = graph;
    this->config = config;
    pheromones.reserve(graph.size());

    for (auto& line : pheromones) {
        line.reserve(graph.size());
        std::fill(begin(line), end(line), config.initPheromoneLevel);
    }
}

template <size_t colonySize>
std::vector<size_t> Ants<colonySize>::operator()(size_t startPoint, size_t endPoint)
{

}

} // ai

#endif // __AI_MMAS_HPP__
