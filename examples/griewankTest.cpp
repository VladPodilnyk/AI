#include <iostream>
#include "utils.hpp"
#include "pso.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test for a griewank function.
     */
    auto griewankFunction = ai::Function(griewankfn, 50, std::make_pair(-600.0, 600.0));
    auto pso = ai::Pso<100>(griewankFunction, 1.2, 2.8, 0.4278, ai::eps);
    auto [gMin, gPos] = pso();
    prettyPrint(gMin, gPos, FuncType::Griewank);

    std::cout << "Check result of PSO:\n";
    std::cout << "Res = " << griewankfn(gPos) << std::endl;

    return 0;
}