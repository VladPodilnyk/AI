#include <iostream>
#include "mmas.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test MMAS algorithm on graph
     * with 55 verticies.
     */
    auto config = ai::AntSystemConfig{3, 4, 16, 30};
    auto aco = ai::Aco(ai::Graph(Parser::getGraphFromFile("yuzSHP55.aco")), config);
    auto best = aco(0, 54);
    std::cout << "The Shortest path from node #0 to node #54:\n";
    //print(best);
    return 0;
}
