#ifndef CIRCUIT_SIMULATOR_H
#define CIRCUIT_SIMULATOR_H

#include <string>
#include <vector>
#include <memory>

// circuit_simulator.h
class Component
{
protected:
    int node1, node2; // Connection nodes
    std::string name;
    double value; // Component value (R, L, C, etc.)

public:
    virtual ~Component() = default;
    virtual double getCurrentContribution(const std::vector<double> &nodeVoltages) = 0;
    virtual double getConductanceContribution() = 0;
};

class Circuit
{
private:
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::vector<double>> conductanceMatrix;
    std::vector<double> currentVector;
    std::vector<double> nodeVoltages;
    int numNodes;

public:
    void addComponent(std::unique_ptr<Component> component);
    void buildSystem();  // Builds conductance matrix and current vector
    void solve();        // Solves system using numerical methods
    void runTransient(); // For time-domain analysis
    void runDC();        // For DC operating point
};

// Example component implementations
class Resistor : public Component
{
public:
    double getCurrentContribution(const std::vector<double> &nodeVoltages) override
    {
        return (nodeVoltages[node1] - nodeVoltages[node2]) / value;
    }

    double getConductanceContribution() override
    {
        return 1.0 / value; // G = 1/R
    }
};

class VoltageSource : public Component
{
public:
    double getCurrentContribution(const std::vector<double> &nodeVoltages) override
    {
        return value; // Returns source value
    }
};

// Define how to build system
void Circuit::buildSystem()
{
    // Initialize matrices
    conductanceMatrix.resize(numNodes, std::vector<double>(numNodes, 0.0));
    currentVector.resize(numNodes, 0.0);

    // Stamp each component's contribution
    for (const auto &component : components)
    {
        // Add conductance contributions to matrix
        // Add current contributions to vector
    }
}

// Default matrix solver set up
void Circuit::solve()
{
    // Using linear algebra library (e.g., Eigen)
    // Solve system: G * V = I
    // where G is conductanceMatrix
    //       V is nodeVoltages (unknown)
    //       I is currentVector

    // Example using Gaussian elimination or iterative methods
    nodeVoltages = solveLinearSystem(conductanceMatrix, currentVector);
}

// Define how to run transient analysis
void Circuit::runTransient()
{
    double timeStep = 0.001; // Example time step
    double endTime = 1.0;    // Simulation end time

    for (double t = 0; t < endTime; t += timeStep)
    {
        updateSources(t); // Update time-varying sources
        buildSystem();    // Rebuild system for current time step
        solve();          // Solve system
        storeResults(t);  // Store results for plotting/analysis
    }
}

#endif // CIRCUIT_SIMULATOR_H