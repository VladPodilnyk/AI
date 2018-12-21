#include <iostream>
#include "utils.hpp"
#include "pso.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test for a ackley function.
     */
    auto sphereFunction = ai::Function(ackleyfn, 50, std::make_pair(-100.0, 100.0));
    auto pso = ai::Pso<60>(sphereFunction, ai::crCoef, ai::sfCoef, ai::inrWeight, ai::eps);
    auto [gMin, gPos] = pso();
    prettyPrint(gMin, gPos, FuncType::Ackley);

    std::cout << "Check result of PSO:\n";
    std::cout << "Res = " << ackleyfn(gPos) << std::endl;

    return 0;
}