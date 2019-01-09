#include <iostream>
#include "mmas.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test MMAS algorithm on graph
     * with 155 verticies.
     */
    auto config = ai::AntSystemConfig{0.86, 1.45, 20.0, 80, 155, 0.28};
    auto aco = ai::Aco(ai::Graph(Parser::getGraphFromFile("yuzSHP155.aco")), config);
    auto best = aco(0, 154);
    std::cout << "The Shortest path from node #0 to node #154:\n";
    print(best);
    return 0;
}
