#include <iostream>
#include "mmas.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test MMAS algorithm on graph
     * with 95 verticies.
     */
    auto config = ai::AntSystemConfig{};
    auto aco = ai::Aco(ai::Graph(Parser::getGraphFromFile("yuzSHP95.aco")), config);
    auto best = aco(0, 94);
    std::cout << "The Shortest path from node #0 to node #94:\n";
    print(best);
    return 0;
}
