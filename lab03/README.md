# Lab03 - Programação Concorrente


### Análise do Desempenho: Multiplicação de Matrizes (Multithreading)

Este projeto tem como objetivo implementar uma solução concorrente para o problema de multiplicação de duas matrizes, coletar informações sobre o seu tempote de execução, e calcular o ganho obtido.


## Estrutura do Projeto

- `mat_seq.c`: Implementa a multiplicação de duas matrizes de forma sequencial.
- `mat_conc.c`: Implementa a multiplicação de duas matrizes utilizando múltiplas threads para realizar o cálculo de forma concorrente.
- `geraMatrizBinario.c`: Gera matrizes aleatórias e salva em arquivos binários, que são utilizados como entrada pelos programas de multiplicação.

## Detalhes de Implementação
Aqui está um exemplo de um arquivo README para o código de multiplicação de matrizes usando threads:

### 1. Divisão do Trabalho entre as Threads

A divisão do trabalho é feita com base nas linhas da primeira matriz (`mat1`):

- O número total de linhas de `mat1` é dividido pelo número de threads (`nthreads`).
- Cada thread é atribuída a um bloco de linhas para processar. O número básico de linhas por thread é calculado como:
  ```c
  blocos = mat1->linha / nthreads;
  ```
- Se houver linhas restantes após a divisão, essas linhas são distribuídas entre as primeiras threads. Isso é controlado pela variável `resto`:
  ```c
  resto = mat1->linha % nthreads;
  ```

### 2. Distribuição e Execução das Threads

- Cada thread recebe um intervalo de linhas para processar, definido por `args[i].inicio` e `args[i].fim`.
- As primeiras `resto` threads recebem uma linha adicional para balancear a carga:
  ```c
  args[i].fim = inicioLinha + blocos + (i < resto ? 1 : 0);
  ```
- As threads são criadas usando a função `pthread_create` e executam a função `multiplicacaoMatrizes`, que realiza a multiplicação de suas linhas atribuídas com todas as colunas da segunda matriz (`mat2`).

## Como Compilar e Executar

### Compilação

Use o `gcc` para compilar os arquivos C. Abaixo estão os comandos de exemplo para compilar cada um dos programas:

```bash
gcc -o mat_seq mat_seq.c -lpthread
gcc -o mat_conc mat_conc.c -lpthread
gcc -o geraMatrizBinario geraMatrizBinario.c
```

### Execução

1. **Gerar Matrizes Aleatórias**:

   Para gerar duas matrizes de dimensões especificadas e salvá-las em arquivos binários:

   ```bash
   ./geraMatrizBinario <arquivo_matriz1> <linhas1> <colunas1> <arquivo_matriz2> <linhas2> <colunas2>
   ```

   Exemplo:

   ```bash
   ./geraMatrizBinario matriz1.bin 100 200 matriz2.bin 200 150
   ```

2. **Multiplicação Sequencial**:

   Para executar a multiplicação de matrizes sequencial:

   ```bash
   ./mat_seq <arquivo_matriz1> <arquivo_matriz2> <arquivo_matriz_saida>
   ```

   Exemplo:

   ```bash
   ./mat_seq matriz1.bin matriz2.bin resultado_seq.bin
   ```

3. **Multiplicação Concorrente**:

   Para executar a multiplicação de matrizes usando threads:

   ```bash
   ./mat_conc <arquivo_matriz1> <arquivo_matriz2> <arquivo_matriz_saida> <numero_de_threads>
   ```

   Exemplo:

   ```bash
   ./mat_conc matriz1.bin matriz2.bin resultado_conc.bin 4
   ```

## Observações

# Medição de Tempo com `GET_TIME`

Este projeto utiliza a função `clock_gettime` para medir o tempo de execução de trechos específicos do código. A macro `GET_TIME` simplifica a medição de tempo em segundos, usando o relógio monotônico do sistema para garantir uma medição precisa e não afetada por ajustes no relógio do sistema.

## Definição da Macro `GET_TIME`

A macro `GET_TIME` é definida para capturar o tempo atual e armazená-lo em uma variável do tipo `double`. Abaixo está a definição da macro:

```c
#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#include <sys/time.h>
#define BILLION 1000000000L

/* A macro agora deve receber um double (não um ponteiro para double) */
#define GET_TIME(now) { \
   struct timespec time; \
   clock_gettime(CLOCK_MONOTONIC, &time); \
   now = time.tv_sec + time.tv_nsec/1000000000.0; \
}
#endif
```

- **`struct timespec time;`**: Declara uma estrutura `timespec` para armazenar o tempo em segundos e nanossegundos.

- **`clock_gettime(CLOCK_MONOTONIC, &time);`**: Preenche a estrutura `time` com o tempo atual do relógio monotônico, que é um relógio que mede o tempo a partir de um ponto fixo e não retrocede.

- **`now = time.tv_sec + time.tv_nsec/1000000000.0;`**: Converte o tempo armazenado em `time.tv_sec` (segundos) e `time.tv_nsec` (nanossegundos) para um float. A fração de segundo é obtida dividindo `time.tv_nsec` por 1 bilhão (número de nanossegundos em um segundo).
