#include <iostream>
#include "utils.hpp"
#include "pso.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test for a sphere function.
     */
    auto sphereFunction = ai::Function(spherefn, 50, std::make_pair(-100.0, 100.0));
    auto pso = ai::Pso<20>(sphereFunction, ai::crCoef, ai::sfCoef, 0.42984, ai::eps);
    auto [gMin, gPos] = pso();
    prettyPrint(gMin, gPos, FuncType::Sphere);

    std::cout << "Check result of PSO:\n";
    std::cout << "Res = " << spherefn(gPos) << std::endl;

    return 0;
}