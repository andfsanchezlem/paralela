#!/bin/bash

for kernel in 3 5 7 9 11 13 15
do
    for n_threads in 1 2 4 6 8
    do
        echo "------------------------------------------" >> mediciones_openmpi.txt
        echo "Imagen en 1080" >> mediciones_openmpi.txt
        echo "Usando un kernel de " $kernel "y" $n_threads "hilos">> mediciones_openmpi.txt
        { time mpirun -np $n_threads --hostfile hostfile mpi_blur_effect 1080.jpg $kernel >/dev/null 2>&1; } 2>> mediciones_openmpi.txt

        echo "------------------------------------------" >> mediciones_openmpi.txt
        echo "Imagen en 720" >> mediciones_openmpi.txt
        echo "Usando un kernel de " $kernel "y" $n_threads "hilos">> mediciones_openmpi.txt
        { time mpirun -np $n_threads --hostfile hostfile mpi_blur_effect 720.jpg $kernel >/dev/null 2>&1; } 2>> mediciones_openmpi.txt

        echo "------------------------------------------" >> mediciones_openmpi.txt
        echo "Imagen en 480" >> mediciones_openmpi.txt
        echo "Usando un kernel de " $kernel "y" $n_threads "hilos">> mediciones_openmpi.txt
        { time mpirun -np $n_threads --hostfile hostfile mpi_blur_effect 480.jpg $kernel >/dev/null 2>&1; } 2>> mediciones_openmpi.txt
    done
done
