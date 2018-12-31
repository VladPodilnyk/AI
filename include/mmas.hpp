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
        explicit Graph(utils::matrix& wages);
        ~Graph() = default;
    private:
        utils::matrix paths;
        utils::matrix pheromones;
};

class Ants
{
    public:
    // coming soon ...
    private:
    // coming soon ...
};

} // ai

#endif // __AI_MMAS_HPP__
