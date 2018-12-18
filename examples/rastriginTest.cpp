#include <iostream>
#include "utils.hpp"
#include "pso.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test for a rastrigin function.
     */
    auto sphereFunction = ai::Function(rastriginfn, 50, std::make_pair(-100.0, 100.0));
    auto pso = ai::Pso<60>(sphereFunction);
    auto [gMin, gPos] = pso();
    prettyPrint(gMin, gPos, FuncType::Rastrigin);

    std::cout << "Check result of PSO:\n";
    std::cout << "Res = " << rastriginfn(gPos) << std::endl;
    return 0;
}