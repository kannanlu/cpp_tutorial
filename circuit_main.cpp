#include "circulator_simulator.h"
#include <memory>
#include <iostream>

int main(int argc, char *argv[])
{
    Circuit c1;
     /* Expected results for this voltage divider:
     * Node 1: 5.0V (source voltage)
     * Node 2: 3.33V (voltage divider)
     * Current through voltage source: -1.67mA
     */
    // Add components
    // Note: The last parameter (0) for VoltageSource is its index in the MNA matrix
    c1.addComponent(std::make_unique<VoltageSource>(1, 0, 5.0, 0)); // 5V source
    c1.addComponent(std::make_unique<Resistor>(1, 2, 1000.0));      // 1kΩ resistor R1
    c1.addComponent(std::make_unique<Resistor>(2, 0, 2000.0));      // 2kΩ resistor R2

    c1.runDC();
    // Print results
    c1.printSolution();

    Circuit c2;
    c2.addComponent(std::make_unique<VoltageSource>(1, 0, 5.0, 0)); // 5V source
    c2.addComponent(std::make_unique<Inductor>(1, 2, 1e-3, 0.001)); // 1mH inductor
    c2.addComponent(std::make_unique<Capacitor>(2, 0, 1e-6, 0.001)); // 1µF capacitor
    c2.runTransient(0.01, 0.001); // Simulate for 10ms with 1ms time step
    // Access and print results
    const auto& results = c2.getResults();
    for (const auto& [time, x] : results) {
        std::cout << "Time: " << time << " s" << std::endl;
        std::cout << "  Node 1 Voltage: " << x[0] << " V" << std::endl;
        std::cout << "  Node 2 Voltage: " << x[1] << " V" << std::endl;
        std::cout << "  Current through Inductor: " << x[2] << " A" << std::endl;
    }
    
    // Save results to a file
    c2.saveResultsToFile("output.txt");
    
    return 0;
}