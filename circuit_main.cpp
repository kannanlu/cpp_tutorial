#include "circulator_simulator.h"
#include <memory>

int main(int argc, char *argv[])
{
    Circuit circuit;

    // Add components
    circuit.addComponent(std::make_unique<VoltageSource>(1, 0, 5.0)); // 5V source
    circuit.addComponent(std::make_unique<Resistor>(1, 2, 1000.0));   // 1kΩ resistor

    // Run simulation
    circuit.runDC();

    // Print results
    circuit.printNodeVoltages();

    return 0;
}