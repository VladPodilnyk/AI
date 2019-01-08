/**
 * file: mmas.cpp
 * synopsis: Implemenation for the MIN MAX Ant System algorithm
 * author: Vladyslav Podilnyk
 */

#include <limits>
#include "mmas.hpp"
#include "randGen.hpp"

namespace ai {

/**
 * Implementation for Graph class
 */
size_t Graph::getPathWeight(const std::vector<size_t>& path)
{
    auto result = size_t{0};
    for (size_t node = 0; node < path.size() - 1; ++node) {
        result += getWeight(path[node], path[node + 1]);
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

std::vector<size_t> Graph::getAdjacentVerticies(const size_t vertex)
{
    auto result = std::vector<size_t>{};
    for (size_t column = 0; column < paths.size(); ++column) {
        if (paths[vertex][column]) {
            result.push_back(column);
        }
    }
    return result;
}


/**
 * Implementation for Aco class
 */
Aco::Aco(const Graph& graph, const AntSystemConfig& config)
{
    this->graph = graph;
    this->config = config;
    shortestPath = std::vector<size_t>{};
    bestPathWeight = std::numeric_limits<size_t>::max();

    for (size_t line = 0; line < graph.size(); ++line) {
        auto vect = std::vector<double>(graph.size());
        std::fill(begin(vect), end(vect), this->config.initPheromoneLevel);
        pheromones.emplace_back(vect);
    }
}

bool Aco::isRouteCompleted(const std::vector<size_t>& route)
{
    return route.front() == startPoint && route.back() == endPoint;
}

std::vector<double> Aco::calculateProbabilities(const size_t vertex,
                                        std::vector<size_t>& adjacentVerticies)
{
    auto result = std::vector<double>{};
    auto sum = 0.0;

    for (const auto& value : adjacentVerticies) {
        auto probability = std::pow(pheromones[vertex][value], config.alpha)
                            / std::pow(graph(vertex, value), config.beta);
        sum += probability;
        result.emplace_back(probability);
    }

    for (auto& value : result) {
        value /= sum;
    }

    return result;
}

size_t Aco::getNextVertex(const size_t vertex)
{
    auto adjacentVerticies = graph.getAdjacentVerticies(vertex);
    auto probabilities = calculateProbabilities(vertex, adjacentVerticies);
    auto total = 0.0;
    auto randValue = rgen::RandGen<double>(std::make_pair(0.0, 1.0)).randValue();

    for (size_t index = 0; index < probabilities.size(); ++index) {
        total += probabilities[index];
        if (total > randValue) {
            return adjacentVerticies[index];
        }
    }

    /**
     * In case we didn't choose any vertex,
     * which is bad, just pick random vertex
     * from the list.
     */
    std::cout << "IS it real??\n";
    return rgen::RandGen<size_t, std::uniform_int_distribution<size_t>>(
                std::make_pair(0, probabilities.size())).randValue();
}

utils::matrix<size_t> Aco::constructSolutions()
{
    auto finishedPathes = utils::matrix<size_t>{};
    auto routes = utils::matrix<size_t>(config.numberOfAnts);
    for (auto& route : routes) {
        route.push_back(startPoint);
    }

    for (size_t iter = 0; iter < config.maxAntMoves; ++iter) {
        for (auto& route : routes) {
            if (isRouteCompleted(route)) {
                continue;
            }

            auto next = getNextVertex(route.back());
            route.push_back(next);
            if (route.back() == endPoint) {
                finishedPathes.push_back(route);
            }
        }
    }
    return finishedPathes;
}

std::pair<size_t, utils::path> Aco::findBest(const utils::matrix<size_t>& routes)
{
    auto min = graph.getPathWeight(routes[0]);
    auto best = size_t{0};

    for (size_t index = 1; index < routes.size(); ++index) {
        auto pathWeight = graph.getPathWeight(routes[index]);
        if (pathWeight < min) {
            min = pathWeight;
            best = index;
        }
    }

    return std::make_pair(min, routes[best]);
}

double Aco::getMaxPheromoneLevel()
{
    return (1.0 / (1 - config.p)) * (1.0 / graph.getPathWeight(shortestPath));
}

double Aco::getMinPheromoneLevel()
{
    return getMaxPheromoneLevel() / config.alpha;
}

double Aco::getPheromone(size_t startPoint, size_t endPoint)
{
    return pheromones[startPoint][endPoint];
}

void Aco::setPheromone(size_t startPoint, size_t endPoint, double value)
{
    pheromones[startPoint][endPoint] = value;
}

void Aco::updatePheromoneLevel(utils::matrix<size_t>& routes)
{
    if (!routes.size()) {
        return;
    }

    auto [currBestWeight, currBestRoute] = findBest(routes);
    if (currBestWeight < bestPathWeight) {
        std::cout << "best route :\n";
        utils::print(currBestRoute);
        shortestPath = currBestRoute;
        bestPathWeight = currBestWeight;
    }

    auto maxPheromoneLevel = getMaxPheromoneLevel();

    for (const auto& route : routes) {
        auto delta = Q / graph.getPathWeight(route);
        for (size_t index = 0; index < route.size() - 1; ++index) {
            auto newVal = getPheromone(route[index], route[index + 1]) + delta;

            if (newVal < maxPheromoneLevel) {
                setPheromone(route[index], route[index + 1], newVal);
                setPheromone(route[index + 1], route[index], newVal);
            } else {
                setPheromone(route[index], route[index + 1], maxPheromoneLevel);
                setPheromone(route[index + 1], route[index], maxPheromoneLevel);
            }
        }
    }
}

void Aco::evaporate()
{
    if (!shortestPath.size()) {
        return;
    }

    auto minPheromoneLevel = getMinPheromoneLevel();

    for (auto& row : pheromones) {
        for (auto& value : row) {
            auto temp = value * (1 - config.p);

            if (temp >= minPheromoneLevel) {
                value = temp;
            } else {
                value = minPheromoneLevel;
            }
        }
    }
}

utils::path Aco::operator()(size_t startPoint, size_t endPoint)
{
    this->startPoint = startPoint;
    this->endPoint = endPoint;
    auto i = 1000;
#if 0
    for (size_t line = 0; line < graph.size(); ++line) {
        for (size_t column = 0; column < graph.size(); ++column) {
            std::cout << graph(line, column) << " ";
        }
        std::cout << "\n";
    }
#else
    while (i) {
        auto finishedRoutes = constructSolutions();
        /*if (finishedRoutes.size()) {
            std::cout << "-----------------------------------------------------------\n";
            for (auto& line : finishedRoutes) {
                utils::print(line);
            }
            std::cout << "-----------------------------------------------------------\n";
        }*/
        updatePheromoneLevel(finishedRoutes);
        evaporate();
        std::cout << i << std::endl;
        --i;
    }
#endif
    return shortestPath;
}

} // ai