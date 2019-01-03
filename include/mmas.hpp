/**
 * file: mmas.hpp
 * synopsis: API for the MIN MAX Ant System algorithm
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
        size_t getPathWeight(const std::vector<size_t>& path);
        size_t getWeight(size_t startNode, size_t endNode);
        size_t operator()(size_t startNode, size_t endNode);
        size_t size() { return paths.size(); };
        size_t size() const { return paths.size(); };
        ~Graph() = default;
    private:
        utils::matrix paths;
};

struct AntSystemConfig
{
    double alpha = 0.0;
    double beta = 0.0;
    double initPheromoneLevel = 0.0;
    size_t numberOfAnts = 30;
};

class Aco
{
    public:
        Aco() = default;
        Aco(const Graph& graph, const AntSystemConfig& config = AntSystemConfig{});
        utils::path operator()(size_t startPoint, size_t endPoint);
        ~Aco() = default;
    private:
        // functions
        utils::matrix constructSolutions();
        void updatePheromoneLevel(utils::matrix& routes);
        void evaporate();

        //data
        Graph graph;
        AntsSystemConfig config;
        utils::matrix pheromones;
        utils::path shortestPath;
        utils::path currentBest;
};

} // ai

#endif // __AI_MMAS_HPP__
