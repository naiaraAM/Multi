#!/bin/bash

num_programs=5;
num_iter=5;
output_file="output.txt";

rm -f $output_file

program_names=("secuential.c" "threads.c" "mpi.c" "omp.c" "omp_errors.c");

for ((i = 0; i < num_programs; i++))
do
    execution_times=()
    echo "Executing ${program_names[i]}"
    case $i in
        0 | 1)
            gcc ${program_names[i]} -lm
            for ((j = 0; j < num_iter; j++))
            do
                output=$(./a.out < input.txt)
                execution_time=$(echo "$output" | grep "Total time" | awk '{print $3}')
                execution_times+=($execution_time)
                echo "Iter $j: $execution_time seconds"
            done
        ;;
        2)
            mpicc ${program_names[i]} -lm
            for ((j = 0; j < num_iter; j++))
            do
                output=$(mpirun --oversubscribe -mca btl_vader_single_copy_mechanism none -np 2 a.out < input.txt)
                execution_time=$(echo "$output" | grep "Total time" | awk '{print $3}')
                execution_times+=($execution_time)
                echo "Iter $j: $execution_time seconds"
            done
        ;;
        3 | 4)
            gcc ${program_names[i]} -lm -fopenmp
            for ((j = 0; j < num_iter; j++))
            do
                output=$(./a.out)
                execution_time=$(echo "$output" | grep "Total time" | awk '{print $3}')
                execution_times+=($execution_time)
                echo "Iter $j: $execution_time seconds"
            done
        ;;
    esac

    sorted_times=($(printf "%s\n" "${execution_times[@]}" | sort -n))
    unset 'sorted_times[0]'
    unset 'sorted_times[-1]'
    total_time=0
    for time in "${sorted_times[@]}"
    do
        total_time=$(echo "$total_time + $time" | bc)
    done
    average_time=$(echo "scale=3; $total_time / ${#sorted_times[@]}" | bc)
    echo "[Average time ${program_names[i]}] $average_time seconds" >> $output_file
done
python3 graphic.py