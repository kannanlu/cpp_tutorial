#ifndef CIRCUIT_SIMULATOR_H
#define CIRCUIT_SIMULATOR_H

#include <string>
#include <vector>
#include <memory> // to allow dynamic memory allocaiton of using smart pointers
#include <Eigen/Dense> // Eigen3 package for linear algebra
#include <fstream>
#include <iostream> // For std::cout, std::cerr
#include <iomanip>  // For std::setw, std::setprecision

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
                       const std::vector<double> &x, int numVoltageSources) = 0;
    virtual bool isVoltageSource() const { return false; }
    int getNode1() const { return node1; } // Getter for node1
    int getNode2() const { return node2; } // Getter for node2
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
    std::vector<std::pair<double, std::vector<double>>> results; // Stores (time, x) pairs

public:
    Circuit() : numNodes(0), numVoltageSources(0) {}
    void addComponent(std::unique_ptr<Component> component); // populate A, z
    void buildSystem();                                      // populate z
    void runTransient(double endTime, double timeStep); // For time-domain analysis
    void runDC();
    void printA(); // For DC operating point
    void printSolution(); // print the x solution
    const std::vector<std::pair<double, std::vector<double>>>& getResults() const;
    void saveResultsToFile(const std::string& filename) const;
    void storeResults(double t); // Store results at time t
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
               const std::vector<double> &x, int numVoltageSources) override
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
               const std::vector<double> &x, int numVoltageSources) override
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

class Capacitor : public Component {
private:
    double prevVoltage; // Voltage across the capacitor at the previous time step
    double timeStep;    // Time step for the simulation

public:
    Capacitor(int n1, int n2, double capacitance, double dt)
        : prevVoltage(0.0), timeStep(dt) {
        node1 = n1;
        node2 = n2;
        value = capacitance; // Capacitance value (C)
    }

    void stamp(std::vector<std::vector<double>> &A, std::vector<double> &z,
               const std::vector<double> &x, int numVoltageSources) override {
        double gc = value / timeStep; // Conductance G_C = C / Δt
        double ic = gc * prevVoltage; // Current source I_C = G_C * V_prev

        // Stamp conductance (similar to a resistor)
        if (node1 > 0) {
            A[node1 - 1][node1 - 1] += gc;
            if (node2 > 0)
                A[node1 - 1][node2 - 1] -= gc;
        }
        if (node2 > 0) {
            if (node1 > 0)
                A[node2 - 1][node1 - 1] -= gc;
            A[node2 - 1][node2 - 1] += gc;
        }

        // Stamp current source (RHS vector z)
        if (node1 > 0)
            z[node1 - 1] -= ic;
        if (node2 > 0)
            z[node2 - 1] += ic;

        // Update previous voltage for the next time step
        prevVoltage = (node1 > 0 ? x[node1 - 1] : 0.0) - (node2 > 0 ? x[node2 - 1] : 0.0);
    }
};

//===----------------------------------------------------------------------===//
// Methods in the Circuit class
//===----------------------------------------------------------------------===//
// Define how to build system
void Circuit::buildSystem()
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
        component->stamp(A, z, x, numVoltageSources);
    }
};

// Default matrix solver set up
// void Circuit::solve()
// {
//     // Solve Ax = z using your preferred linear algebra method
//     x = LinearSolver->solveLinearSystem(A, z);
//     // First numNodes elements of x are node voltages
//     // Remaining elements are branch currents through voltage sources
// };

void Circuit::runDC() {
    // Build the MNA system for DC analysis
    buildSystem();

    // Convert A and z to Eigen matrices
    Eigen::MatrixXd eigenA(A.size(), A[0].size());
    Eigen::VectorXd eigenZ(z.size());

    // Copy data from std::vector to Eigen matrices
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < A[i].size(); ++j) {
            eigenA(i, j) = A[i][j];
        }
    }
    for (size_t i = 0; i < z.size(); ++i) {
        eigenZ(i) = z[i];
    }

    // Solve the system using Eigen's LU decomposition
    Eigen::VectorXd eigenX = eigenA.lu().solve(eigenZ);

    // Copy the solution back to x
    x.resize(eigenX.size());
    for (size_t i = 0; i < eigenX.size(); ++i) {
        x[i] = eigenX(i);
    }
}

void Circuit::runTransient(double endTime, double timeStep) {
    // Clear previous results
    results.clear();

    // Initialize time
    double t = 0.0;

    // Run transient simulation
    while (t < endTime) {
        // Build the MNA system for the current time step
        buildSystem();

        // Convert A and z to Eigen matrices
        Eigen::MatrixXd eigenA(A.size(), A[0].size());
        Eigen::VectorXd eigenZ(z.size());

        // Copy data from std::vector to Eigen matrices
        for (size_t i = 0; i < A.size(); ++i) {
            for (size_t j = 0; j < A[i].size(); ++j) {
                eigenA(i, j) = A[i][j];
            }
        }
        for (size_t i = 0; i < z.size(); ++i) {
            eigenZ(i) = z[i];
        }

        // Solve the system using Eigen's LU decomposition
        Eigen::VectorXd eigenX = eigenA.lu().solve(eigenZ);

        // Copy the solution back to x
        x.resize(eigenX.size());
        for (size_t i = 0; i < eigenX.size(); ++i) {
            x[i] = eigenX(i);
        }

        // Store or process the results (e.g., save node voltages for plotting)
        storeResults(t);

        // Update time
        t += timeStep;
    }
}

void Circuit::printA() {
    std::cout << "MNA Matrix (A):" << std::endl;
    for (const auto& row : A) {
        for (const auto& element : row) {
            std::cout << std::setw(10) << std::setprecision(4) << element << " ";
        }
        std::cout << std::endl;
    }
};

void Circuit::printSolution() {
    std::cout << "Solution (x):" << std::endl;
    
    // Print node voltages
    std::cout << "Node Voltages:" << std::endl;
    for (int i = 0; i < numNodes; ++i) {
        std::cout << "  Node " << (i + 1) << ": " << std::setw(10) << std::setprecision(4) << x[i] << " V" << std::endl;
    }
    
    // Print branch currents
    std::cout << "Branch Currents:" << std::endl;
    for (int i = numNodes; i < x.size(); ++i) {
        std::cout << "  Current through voltage source " << (i - numNodes + 1) << ": " 
                  << std::setw(10) << std::setprecision(4) << x[i] << " A" << std::endl;
    }
};

void Circuit::storeResults(double t) {
    // Store the current time and solution vector
    results.push_back(std::make_pair(t, x));
};

const std::vector<std::pair<double, std::vector<double>>>& Circuit::getResults() const {
    return results;
};

void Circuit::saveResultsToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // Write header
    file << "Time";
    for (int i = 0; i < numNodes; ++i) {
        file << ", Node" << (i + 1);
    }
    for (int i = numNodes; i < x.size(); ++i) {
        file << ", Current" << (i - numNodes + 1);
    }
    file << "\n";

    // Write data
    for (const auto& [time, x] : results) {
        file << time;
        for (double value : x) {
            file << ", " << value;
        }
        file << "\n";
    }

    file.close();
};

void Circuit::addComponent(std::unique_ptr<Component> component) {
    // Update the number of nodes if necessary
    numNodes = std::max(numNodes, std::max(component->getNode1(), component->getNode2()));
    // Add the component to the list
    components.push_back(std::move(component));
};

// class LinearSolver
// {
// public:
//     virtual std::vector<double> solveLinearSystem(const std::vector<std::vector<double>> &A, const std::vector<double> &z) = 0;
// };


#endif // CIRCUIT_SIMULATOR_H