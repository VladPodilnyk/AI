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
        explicit Graph(const utils::matrix<size_t>& wages) { paths = wages; };
        size_t getPathWeight(const std::vector<size_t>& path);
        size_t getWeight(size_t startNode, size_t endNode);
        size_t operator()(size_t startNode, size_t endNode);
        std::vector<size_t> getAdjacentVerticies(const size_t vertex);
        size_t size() { return paths.size(); };
        size_t size() const { return paths.size(); };
        ~Graph() = default;
    private:
        utils::matrix<size_t> paths;
};

struct AntSystemConfig
{
    double alpha = 1.0;
    double beta = 1.0;
    double initPheromoneLevel = 10.0;
    size_t numberOfAnts = 30;
    size_t maxAntMoves = 55;
    double p = 0.08;
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

        // steps of the algorithm
        utils::matrix<size_t> constructSolutions();
        void updatePheromoneLevel(utils::matrix<size_t>& routes);
        void evaporate();

        // helpers
        bool isRouteCompleted(const std::vector<size_t>& route);
        size_t getNextVertex(const size_t vertex);
        std::vector<double> calculateProbabilities(const size_t vertex);
        utils::path findBest(const utils::matrix<size_t>& routes);
        double getMaxPheromoneLevel();
        double getMinPheromoneLevel();

        //data
        Graph graph;
        AntSystemConfig config;
        utils::matrix<double> pheromones;
        utils::path shortestPath;
        //utils::path currentBest;
        size_t startPoint = 0;
        size_t endPoint = 0;
        double Q = 100;
};

} // ai

#endif // __AI_MMAS_HPP__
