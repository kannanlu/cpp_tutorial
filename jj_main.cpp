#include "circulator_simulator.h"
#include <iostream>

int main() {
    // Create a circuit
    Circuit circuit;

    // Define the nodes
    int node1 = 1; // Node 1 (connected to the voltage source and JJ)
    int node2 = 0; // Node 0 (ground)
    int phaseNode = 2; // Phase node for the Josephson Junction

    // Add a voltage source (e.g., 1 V) between node1 and node2
    double voltage = 1.0e-9; // Voltage in volts
    int voltageSourceIndex = 0; // Index for the voltage source
    circuit.addComponent(std::make_unique<VoltageSource>(node2, node1, voltage, voltageSourceIndex));

    // Add a Josephson Junction between node1 and node2
    double criticalCurrent = 1e-6; // Critical current in Amperes (1 µA)
    double resistance = 100.0; // Resistance in Ohms (100 Ω)
    double capacitance = 1e-12; // Capacitance in Farads (1 pF)
    double timeStep = 1e-12; // Time step in seconds (1 ps)
    circuit.addComponent(std::make_unique<JosephsonJunction>(node1, node2, phaseNode, criticalCurrent, resistance, capacitance, timeStep));

    // Run transient simulation
    double endTime = 1e-9; // End time in seconds (1 ns)
    circuit.runTransient_jj(endTime, timeStep);

    // Save the results to a file
    circuit.saveResultsToFile("jj_transient_results.txt");

    std::cout << "Transient simulation completed. Results saved to 'jj_transient_results.csv'." << std::endl;

    return 0;
}