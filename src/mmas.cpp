/**
 * file: mmas.cpp
 * synopsis: Implemenation for the MIN MAX Ant System algorithm
 * author: Vladyslav Podilnyk
 */

#include "mmas.hpp"

namespace ai {

/**
 * Implementation for Graph class
 */
size_t Graph::getPathWeight(const std::vector<size_t>& path)
{
    auto result = size_t{0};
    for (size_t node = 0; node < path.size() - 1; ++node) {
        result = paths[node][node + 1];
    }
    return result;
}

size_t Graph::getWeight(size_t startNode, size_t endNode)
{
    return paths[startNode][endNode]; 
}

size_t Graph::operator()(size_t startNode, size_t endNode)
{
    return paths[startNode][endNode];
}


/**
 * Implementation for Aco class
 */
Aco::Aco(const Graph& graph, const AntSystemConfig& config)
{
    this->graph = graph;
    this->config = config;
    pheromones.reserve(graph.size());

    for (auto& line : pheromones) {
        line.reserve(graph.size());
        std::fill(begin(line), end(line), this->config.initPheromoneLevel);
    }
}

utils::matrix Aco::constructSolutions()
{
    auto finishedPathes = utils::matrix{};
    for (size_t iter = 0; iter < config.maxAntMoves; ++iter) {
        for (size_t route = 0; route < config.numberOfAnts; ++route) {
            // smth strage here!!
        }
    }
    return finishedPathes;
}

void Aco::updatePheromoneLevel(utils::matrix& routes)
{

}

void Aco::evaporate()
{

}

utils::path Aco::operator()(size_t startPoint, size_t endPoint)
{

}

} // ai