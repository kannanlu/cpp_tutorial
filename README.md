# Circuit Simulator

This project implements a circuit simulator using Modified Nodal Analysis (MNA) for both DC and transient (time-domain) analysis. It supports basic components such as resistors, capacitors, inductors, and voltage sources.

## Table of Contents

- [Getting Started](#getting-started)
- [CMake Build](#cmake-build)
- [Gnuplot Visualization](#gnuplot-visualization)
- [MNA Time-Domain Solution](#mna-time-domain-solution)
- [Capacitor and Inductor Implementation](#capacitor-and-inductor-implementation)
- [Required Packages](#required-packages)

---

## Getting Started

1. Clone the repository:

   ```bash
   git clone git@your_remote_repo_name
   ```

2. Build and run the project using CMake (see [CMake Build](#cmake-build)).

---

## CMake Build

Instead of manually compiling the project, it is recommended to use `cmake` to automate the build process.

### Steps:

```bash
mkdir build
cd build
cmake ..
make
```

---

## Gnuplot Visualization

The simulation results are saved in `output.txt` and can be visualized using `gnuplot`. A shell script (`plot.sh`) is provided to automate the plotting process.

### Steps:

1. Run the simulation:

   ```bash
   ./circuit
   ```

2. Plot the results:
   ```bash
   ./plot.sh
   ```

The plot will be saved as `lc_oscillation.png`.

---

## MNA Time-Domain Solution

The **Modified Nodal Analysis (MNA)** is used to solve the circuit in both DC and transient (time-domain) analysis. Here’s how the time-domain solution is implemented:

1. **System Construction**:

   - The MNA system is built by stamping the contributions of each component into the system matrix \( A \) and the right-hand side (RHS) vector \( z \).
   - The system matrix \( A \) combines the conductance matrix \( G \) and the incidence matrix \( B \), while the RHS vector \( z \) includes the current sources and voltage source contributions.

2. **Time-Domain Simulation**:

   - For transient analysis, the circuit is solved at each time step using a **backward Euler** method.
   - The time step \( \Delta t \) is fixed, and the solution vector \( x \) (containing node voltages and branch currents) is updated iteratively.

3. **Component Stamping**:

   - Each component (resistor, capacitor, inductor, voltage source) stamps its contribution into the MNA system:
     - **Resistor**: Stamps conductance \( G = \frac{1}{R} \) into \( A \).
     - **Capacitor**: Stamps conductance \( G*C = \frac{C}{\Delta t} \) and current source \( I_C = G_C \cdot V*{\text{prev}} \) into \( A \) and \( z \).
     - **Inductor**: Stamps conductance \( G*L = \frac{\Delta t}{L} \) and current source \( I_L = I*{\text{prev}} \) into \( A \) and \( z \).
     - **Voltage Source**: Stamps entries into \( A \) and \( z \) to enforce the voltage constraint.

4. **Solution**:
   - The system \( A x = z \) is solved at each time step using **Eigen’s LU decomposition**.
   - The solution vector \( x \) is stored for each time step, allowing the results to be saved and plotted.

---

## Capacitor and Inductor Implementation

### Capacitor

The capacitor is implemented using the **backward Euler** method for time-domain analysis:

1. **Conductance**:

   - The equivalent conductance of the capacitor is \( G_C = \frac{C}{\Delta t} \), where \( C \) is the capacitance and \( \Delta t \) is the time step.

2. **Current Source**:

   - The current source \( I*C = G_C \cdot V*{\text{prev}} \) represents the voltage across the capacitor at the previous time step.

3. **Stamping**:

   - The conductance \( G_C \) is stamped into the system matrix \( A \).
   - The current source \( I_C \) is stamped into the RHS vector \( z \).

4. **Update**:
   - The voltage across the capacitor is updated for the next time step based on the solution vector \( x \).

### Inductor

The inductor is also implemented using the **backward Euler** method:

1. **Conductance**:

   - The equivalent conductance of the inductor is \( G_L = \frac{\Delta t}{L} \), where \( L \) is the inductance and \( \Delta t \) is the time step.

2. **Current Source**:

   - The current source \( I*L = I*{\text{prev}} \) represents the current through the inductor at the previous time step.

3. **Stamping**:

   - The conductance \( G_L \) is stamped into the system matrix \( A \).
   - The current source \( I_L \) is stamped into the RHS vector \( z \).

4. **Update**:
   - The current through the inductor is updated for the next time step based on the solution vector \( x \).

---

## Required Packages

### Eigen3

The project uses **Eigen3** for linear algebra operations. To install Eigen3 on macOS:

```bash
brew install eigen
```

The Eigen3 headers are typically located at `/opt/homebrew/Cellar/eigen/3.4.0_1/include/eigen3/` and are included in the CMake configuration.

---

## Example: LC Oscillation Circuit

An LC circuit (inductor and capacitor in series) is simulated to demonstrate the time-domain solution:

1. **Components**:

   - A 1mH inductor and a 1µF capacitor are connected in series with a 5V voltage source.
   - The circuit is simulated for 10ms with a time step of 1ms.

2. **Results**:
   - The voltage at Node 1 and Node 2, as well as the current through the inductor, are saved to `output.txt`.
   - The results are plotted using `gnuplot` to visualize the oscillation.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
