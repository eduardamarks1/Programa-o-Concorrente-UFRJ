#!/bin/bash

executions=5
output_file="results.txt"

# Limpar resultados anteriores
echo "" > $output_file

# Tamanhos das matrizes
sizes=("500" "1000" "2000")
threads=("1" "2" "4" "8")

for size in "${sizes[@]}"; do
    # Gerar matrizes de entrada
    ./geraMatrizBinario $size $size "matrix1_${size}.bin"
    ./geraMatrizBinario $size $size "matrix2_${size}.bin"

    # Executar versão sequencial
    echo "Matrix Size: ${size}x${size} - Sequencial" >> $output_file
    for i in $(seq 1 $executions); do
        ./mat_seq "matrix1_${size}.bin" "matrix2_${size}.bin" "output_${size}.bin" >> $output_file
    done

    # Executar versão concorrente com diferentes números de threads
    for t in "${threads[@]}"; do
        echo "Matrix Size: ${size}x${size} - ${t} Threads" >> $output_file
        for i in $(seq 1 $executions); do
            ./mat_conc "matrix1_${size}.bin" "matrix2_${size}.bin" "output_${size}.bin" $t >> $output_file
        done
    done
done

