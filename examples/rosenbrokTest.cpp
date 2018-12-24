#include <iostream>
#include "utils.hpp"
#include "pso.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test for a rosenbrok function.
     */
    auto rosenbrockFunction = ai::Function(rosenbrokfn, 30, std::make_pair(-5.0, 10.0));
    auto pso = ai::Pso<60>(rosenbrockFunction, 1.7, 1.3, 0.82984, ai::eps);
    auto [gMin, gPos] = pso();
    prettyPrint(gMin, gPos, FuncType::Rosenbrok);

    std::cout << "Check result of PSO:\n";
    std::cout << "Res = " << rosenbrokfn(gPos) << std::endl;

    return 0;
}