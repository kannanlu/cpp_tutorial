#include "circulator_simulator.h"
#include <memory>
#include <iostream>

int main(int argc, char *argv[])
{
    Circuit circuit;

    // Add components
    // Note: The last parameter (0) for VoltageSource is its index in the MNA matrix
    circuit.addComponent(std::make_unique<VoltageSource>(1, 0, 5.0, 0)); // 5V source
    circuit.addComponent(std::make_unique<Resistor>(1, 2, 1000.0));      // 1kΩ resistor R1
    circuit.addComponent(std::make_unique<Resistor>(2, 0, 2000.0));      // 2kΩ resistor R2

    // Print A matrix
    circuit.printA();

    // Run DC analysis
    circuit.solve();

    // Print results
    std::cout << "Node Voltages:" << std::endl;
    circuit.printSolution();

    /* Expected results for this voltage divider:
     * Node 1: 5.0V (source voltage)
     * Node 2: 3.33V (voltage divider)
     * Current through voltage source: -1.67mA
     */

    return 0;
}