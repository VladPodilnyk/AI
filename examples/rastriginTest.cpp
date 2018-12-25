#include <iostream>
#include "utils.hpp"
#include "pso.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test for a rastrigin function.
     */
    auto rastriginFunction = ai::Function(rastriginfn, 30, std::make_pair(-5.12, 5.12));
    auto pso = ai::Pso<60>(rastriginFunction, 0.2, 3.8, 0.12984, ai::eps);
    auto [gMin, gPos] = pso();
    prettyPrint(gMin, gPos, FuncType::Rastrigin);

    std::cout << "Check result of PSO:\n";
    std::cout << "Res = " << rastriginfn(gPos) << std::endl;
    return 0;
}