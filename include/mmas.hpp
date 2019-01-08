/**
 * file: mmas.hpp
 * synopsis: API for the MIN MAX Ant System algorithm
 * author: Vladyslav Podilnyk
 */

#ifndef __AI_MMAS_HPP__
#define __AI_MMAS_HPP__

#include <algorithm>
#include <utility>
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
    double alpha = 1.6;
    double beta = 1.4;
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
        std::vector<double> calculateProbabilities(const size_t vertex,
                                            std::vector<size_t>& adjacentVerticies);
        std::pair<size_t, utils::path> findBest(const utils::matrix<size_t>& routes);
        double getMaxPheromoneLevel();
        double getMinPheromoneLevel();
        double getPheromone(size_t startPoint, size_t endPoint);
        void setPheromone(size_t startPoint, size_t endPoint, double value);

        //data
        Graph graph;
        AntSystemConfig config;
        utils::matrix<double> pheromones;
        utils::path shortestPath;
        size_t bestPathWeight;
        size_t startPoint = 0;
        size_t endPoint = 0;
        double Q = 100;
};

} // ai

#endif // __AI_MMAS_HPP__
