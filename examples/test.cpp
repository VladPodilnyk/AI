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
    auto pso = ai::Pso<60>(sphereFunction);
    auto [gMin, gPos] = pso();
    prettyPrint(gMin, gPos, FuncType::Sphere);
    /*
    std::cout << "Global Min = " <<gMin << std::endl;
    std::cout << '\n';

    for (const auto& i : gPos) {
        std::cout << " " << i;
    }
    std::cout << '\n';
*/
    std::cout << "Res = " << spherefn(gPos) << std::endl;

    /**
     * Test for a ackley function
     */


    /**
     * Test for a griewank function
     */


    /**
     * Test for a rastrigin function
     */

    /**
     * Test for a ackley function
     */

    /**
     * Test for a rosenbrok function
     */
    return 0;
}