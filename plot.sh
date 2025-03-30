#!/bin/bash

# Define the path to the data file for plotting
output_file="build/output.txt"

# Plot Node 1 voltage vs time using gnuplot
gnuplot << EOF
set terminal pngcairo enhanced size 800,600
set output 'lc_oscillation.png'
set title "LC Oscillation: Node 1 Voltage vs Time"
set xlabel "Time (s)"
set ylabel "Node 1 Voltage (V)"
set grid
plot "$output_file" using 1:2 with lines title "Node 1 Voltage"
EOF

echo "Plot saved as lc_oscillation.png" 