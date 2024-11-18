#ifndef CIRCUIT_SIMULATOR_H
#define CIRCUIT_SIMULATOR_H

#include <string>
#include <vector>
#include <memory> // to allow dynamic memory allocaiton of using smart pointers

// std::map<std::string, double> constants; // constants = {"resistor": 1, "capacitor": 1}

// circuit_simulator.h
// Base class for every component in the circuit,
// right now the circuit contains L, C , R, V, I components,
// if more exotic components are needed, one need to considers more complicated structure of the component
class Component
{
protected:            // protected members can be accessed by derived classes, rather than private members
    int node1, node2; // Connection nodes
    std::string name;
    double value;   // Component value (R, L, C, etc.)
    int voltageIdx; // null: not a voltage source, otherwise: Index in the MNA matrix for voltage sources

public:
    virtual ~Component() = default;
    // Modified interface for MNA
    virtual void stamp(std::vector<std::vector<double>> &A, std::vector<double> &z,
                       const std::vector<double> &x) = 0;
    virtual bool isVoltageSource() const { return false; }
};

// 0 - R01 - 1 - R12 -2 - R23 - 3 - V1 - 0
// A = [ [ 1/ R01 + 1/ R12] ]

// Circuit class for holding the components and solve for the circuit
class Circuit
{
private:
    std::vector<std::unique_ptr<Component>> components; // a vector of unique pointers to Component  [*componet1, *component2 ...]
    std::vector<std::vector<double>> A;                 // MNA matrix (combines G and B matrices)   [ [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0] ]
    std::vector<double> z;                              // RHS vector
    std::vector<double> x;                              // Solution vector (voltages and currents)
    int numNodes;                                       // N
    int numVoltageSources;                              // M

public:
    Circuit() : numNodes(0), numVoltageSources(0) {}
    void addComponent(std::unique_ptr<Component> component); // populate A, z
    void buildSystem();                                      // populate z
    void solve();                                            // linear solver
    void runTransient();                                     // For time-domain analysis
    void runDC();
    void printA(); // For DC operating point
};

//===----------------------------------------------------------------------===//
// Derived classes of the component class
//===----------------------------------------------------------------------===//
// Example component implementations, resistor component
class Resistor : public Component
{
public:
    Resistor(int n1, int n2, double resistance)
    {
        node1 = n1;
        node2 = n2;
        value = resistance;
    }

    void stamp(std::vector<std::vector<double>> &A, std::vector<double> &z,
               const std::vector<double> &x) override
    {
        double g = 1.0 / value;
        if (node1 > 0)
        {
            A[node1 - 1][node1 - 1] += g;
            if (node2 > 0)
                A[node1 - 1][node2 - 1] -= g;
        }
        if (node2 > 0)
        {
            if (node1 > 0)
                A[node2 - 1][node1 - 1] -= g;
            A[node2 - 1][node2 - 1] += g;
        }
    }
};

// Example component implementation, voltage source component
class VoltageSource : public Component
{
public:
    VoltageSource(int n1, int n2, double voltage, int vIdx)
    {
        node1 = n1;
        node2 = n2;
        value = voltage;
        voltageIdx = vIdx;
    }

    void stamp(std::vector<std::vector<double>> &A, std::vector<double> &z,
               const std::vector<double> &x) override
    {
        int n = A.size() - numVoltageSources;
        // Stamp B matrix entries
        if (node1 > 0)
        {
            A[node1 - 1][n + voltageIdx] += 1.0;
            A[n + voltageIdx][node1 - 1] += 1.0;
        }
        if (node2 > 0)
        {
            A[node2 - 1][n + voltageIdx] -= 1.0;
            A[n + voltageIdx][node2 - 1] -= 1.0;
        }
        // Stamp source value
        z[n + voltageIdx] = value;
    }

    bool isVoltageSource() const override { return true; }
};

class Capacitor : public Component

                  //===----------------------------------------------------------------------===//
                  // Methods in the Circuit class
                  //===----------------------------------------------------------------------===//
                  // Define how to build system
                  void
                  Circuit::buildSystem()
{
    // Count voltage sources
    numVoltageSources = 0;
    for (const auto &component : components)
    {
        if (component->isVoltageSource())
            numVoltageSources++;
    }

    // Size of MNA matrix is (nodes + voltage sources)
    int size = numNodes + numVoltageSources;
    A.resize(size, std::vector<double>(size, 0.0));
    z.resize(size, 0.0);
    x.resize(size, 0.0);

    // Stamp each component's contribution
    for (const auto &component : components)
    {
        component->stamp(A, z, x);
    }
}

// Default matrix solver set up
void Circuit::solve()
{
    // Solve Ax = z using your preferred linear algebra method
    x = LinearSolver->solveLinearSystem(A, z);
    // First numNodes elements of x are node voltages
    // Remaining elements are branch currents through voltage sources
}

class LinearSolver
{
public:
    virtual std::vector<double> solveLinearSystem(const std::vector<std::vector<double>> &A, const std::vector<double> &z) = 0;
};

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