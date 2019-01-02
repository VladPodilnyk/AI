/**
 * file: Graph.cpp
 * synopsis: Implemenation for Graph class
 * author: Vladyslav Podilnyk
 */

#include <algorithm>
#include "mmas.hpp"

namespace ai {

Graph::Graph(const utils::matrix& wages)
{
    paths = wages;
    for (auto& items : pheromones) {
        items.reserve(paths[0].size());
        std::fill(begin(items), end(items), 0);
    }
}

size_t Graph::getPheremoneLevel(size_t startNode, size_t endNode)
{
    return pheromones[startNode][endNode];
}

void Graph::updatePheremoneLevel(size_t startNode, size_t endNode, size_t value)
{
    paths[startNode][endNode] = value;
}

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

size_t Graph::size()
{
    return paths.size();
}

} // ai
