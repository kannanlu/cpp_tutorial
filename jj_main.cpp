#include "circulator_simulator.h"
#include <iostream>
#include <cmath>

const double C_TO_IC = 2 * 3.14 * pow((1 / (2 * 3.14 * 200e9)), 2.0) / 2.067833848e-15;

int main() {
    // Create a circuit
    Circuit circuit;

    // Define the nodes
    int node1 = 1; // Node 1 (connected to the voltage source and JJ)
    int node2 = 0; // Node 0 (ground)
    int phaseNode = 2; // Phase node for the Josephson Junction

    // Add a voltage source (e.g., 1 V) between node1 and node2
    double voltage = 1.8e-3; // Voltage in volts
    int voltageSourceIndex = 0; // Index for the voltage source
    circuit.addComponent(std::make_unique<VoltageSource>(node2, node1, voltage, voltageSourceIndex));

    // Add a Josephson Junction between node1 and node2
    double criticalCurrent = 1e-9; // Critical current in Amperes
    double resistance = 1.0; // Resistance in Ohms
    double capacitance = C_TO_IC * criticalCurrent; // Capacitance in Farads
    double timeStep = 0.01e-12; // Time step in seconds 
    circuit.addComponent(std::make_unique<JosephsonJunction>(node1, node2, phaseNode, criticalCurrent, resistance, capacitance, timeStep));

    // Run transient simulation
    double endTime = 1e-12; // End time in seconds (1 ns)
    circuit.runTransient_jj(endTime, timeStep);

    // Save the results to a file
    circuit.saveResultsToFile("jj_transient_results.txt");

    std::cout << "Transient simulation completed. Results saved to 'jj_transient_results.txt'." << std::endl;

    return 0;
}