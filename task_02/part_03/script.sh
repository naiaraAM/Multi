#!/bin/bash


num_programs=5;
num_iter=5;
execution_times=();

program_names=("secuential.c" "threads.c" "mpi.c" "omp.c" "omp_errors.c");

for ((i = 0; i < num_programs; i++))
do
    echo "Executing ${program_names[i]}"
    case $i in
        0 | 1)
            gcc ${program_names[i]} -lm
            for ((j = 0; j < num_iter; j++))
            do
                echo "Iter $j"
                output=$(./a.out < input.txt)
                execution_time=$(echo "$output" | grep "Total time" | awk '{print $3}')
                execution_times+=($execution_time)
            done
        ;;
        2)
            mpicc ${program_names[i]} -lm
            for ((j = 0; j < num_iter; j++))
            do
                echo "Iter $j"
                output=$(mpirun -np 2 ./a.out < input.txt)
                execution_time=$(echo "$output" | grep "Total time" | awk '{print $3}')
                execution_times+=($execution_time)
            done
        ;;
        3 | 4)
            gcc ${program_names[i]} -lm -fopenmp
            for ((j = 0; j < num_iter; j++))
            do
                echo "Iter $j"
                output=$(./a.out)
                execution_time=$(echo "$output" | grep "Total time" | awk '{print $3}')
                execution_times+=($execution_time)
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
    echo "[Average time ${program_names[i]}] $average_time seconds"
done