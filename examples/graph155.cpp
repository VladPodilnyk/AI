#include <iostream>
#include "mmas.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test MMAS algorithm on graph
     * with 195 verticies.
     */
    auto config = ai::AntSystemConfig{};
    auto aco = ai::Aco(ai::Graph(Parser::getGraphFromFile("yuzSHP155.aco")), config);
    auto best = aco(0, 154);
    std::cout << "The Shortest path from node #0 to node #154:\n";
    print(best);
    return 0;
}
