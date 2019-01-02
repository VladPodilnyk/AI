/**
 * file: mmas.hpp
 * synopsis: Implemenation for the MIN MAX Ant System algorithm
 * author: Vladyslav Podilnyk
 */

#ifndef __AI_MMAS_HPP__
#define __AI_MMAS_HPP__

#include "utils.hpp"

namespace ai {

class Graph
{
    public:
        Graph() = default;
        explicit Graph(const utils::matrix& wages);
        size_t getPheremoneLevel(size_t startNode, size_t endNode);
        void updatePheremoneLevel(size_t startNode, size_t endNode, size_t value);
        size_t getPathWeight(const std::vector<size_t>& path);
        size_t getWeight(size_t startNode, size_t endNode);
        size_t size();
        ~Graph() = default;
    private:
        utils::matrix paths;
        utils::matrix pheromones;
};

struct AntSystemConfig
{
    double alpha = 0.0;
    double beta = 0.0;
};

template <size_t colonySize>
class Ants
{
    public:
        Ants() = default;
        Ants(const Graph& graph);
        std::vector<size_t> operator()(size_t startPoint, size_t endPoint,
                                        AntSystemConfig config = AntSystemConfig{});
        ~Ants() = default;
    private:
        Graph graph;
        std::array<size_t, colonySize> antsColony;
        std::vector<size_t> shortestPath;
};

template <size_t colonySize>
Ants<colonySize>::Ants(const Graph& graph)
{
    this->graph = graph;
    for (size_t line = 0; line < this->graph.size(); ++line) {
        // pass
    }
}

template <size_t colonySize>
std::vector<size_t> Ants<colonySize>::operator()(size_t startPoint, size_t endPoint,
                                                AntSystemConfig config)
{

}

} // ai

#endif // __AI_MMAS_HPP__
