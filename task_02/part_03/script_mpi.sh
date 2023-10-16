#!/bin/bash

# Check if the number of processors is provided as a command-line argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number_of_processors>"
    exit 1
fi

# Get the number of processors from the command-line argument
num_processors="$1"

# Número de ejecuciones
num_executions=5
execution_times=()
deviations=()

echo "Number of processors: $num_processors"

# Compile
mpicc mpi.c -lm

# Execute program num_executions times
for ((i = 1; i <= num_executions; i++))
do
    output=$(timeout 7 mpirun --oversubscribe -mca btl_vader_single_copy_mechanism none -np "$num_processors" a.out < input.txt | grep -E "Total time|Deviation")
    execution_time=$(echo "$output" | grep "Total time" | awk '{print $3}')
    deviation=$(echo "$output" | grep "Deviation" | awk '{print $2}')
    execution_times+=($execution_time)
    deviations+=($deviation)
done

# Sort times
sorted_times=($(printf "%s\n" "${execution_times[@]}" | sort -n))

# Discard highest and lowest times
unset 'sorted_times[0]'
unset 'sorted_times[-1]'

# Calculate average time
total_time=0
for time in "${sorted_times[@]}"
do
    total_time=$(echo "$total_time + $time" | bc)
done
average_time=$(echo "scale=3; $total_time / ${#sorted_times[@]}" | bc)

# Calculate the average deviation
total_deviation=0
for deviation in "${deviations[@]}"
do
    total_deviation=$(echo "$total_deviation + $deviation" | bc)
done
average_deviation=$(echo "scale=3; $total_deviation / ${#deviations[@]}" | bc)

# Print results
echo "[Average time] $average_time seconds"
echo "[Average Deviation] $average_deviation"