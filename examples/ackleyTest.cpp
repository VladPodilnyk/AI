#include <iostream>
#include "utils.hpp"
#include "pso.hpp"

using namespace ai::utils;

int main()
{
    /**
     * Test for a ackley function.
     */
    auto ackleyFunction = ai::Function(ackleyfn, 20, std::make_pair(-32.768, 32.768));
    auto pso = ai::Pso<40>(ackleyFunction, 1.28, 2.4949, 0.72984);
    auto [gMin, gPos] = pso();
    prettyPrint(gMin, gPos, FuncType::Ackley);

    std::cout << "Check result of PSO:\n";
    std::cout << "Res = " << ackleyfn(gPos) << std::endl;

    return 0;
}