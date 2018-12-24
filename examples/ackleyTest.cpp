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
    auto pso = ai::Pso<40>(ackleyFunction, 2.0, 2.0, 0.72984, ai::eps);
    auto [gMin, gPos] = pso();
    prettyPrint(gMin, gPos, FuncType::Ackley);

    std::cout << "Check result of PSO:\n";
    std::cout << "Res = " << ackleyfn(gPos) << std::endl;

    return 0;
}