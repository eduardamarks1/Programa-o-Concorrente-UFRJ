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

Claro! Aqui está uma versão atualizada do README com instruções adicionais sobre permissões e compilação dos arquivos necessários:

---

# Análise de desempenho 

Este projeto inclui um script adiiconal para realizar a análise de desempenho para a multiplicação de matrizes usando implementações sequenciais e concorrentes. O script gera matrizes de diferentes tamanhos, executa o código de multiplicação de matrizes sequencial e concorrente, e coleta os tempos de execução. O script realiza as seguintes tarefas:

1. Gera matrizes de entrada com tamanhos especificados.
2. Executa o programa de multiplicação de matrizes em modo sequencial.
3. Executa o programa de multiplicação de matrizes em modo concorrente com diferentes números de threads.
4. Coleta e salva os tempos de execução em um arquivo de saída no formato txt.

## Estrutura do Script

O script `roda_testes.sh` realiza as seguintes etapas:

### 1. Configurações Iniciais

```bash
#!/bin/bash

executions=5
output_file="results.txt"

# Limpar resultados anteriores
echo "" > $output_file

# Tamanhos das matrizes
sizes=("500" "1000" "2000")
threads=("1" "2" "4" "8")
```

- `executions=5`: Define o número de execuções para cada configuração de teste.
- `output_file="results.txt"`: Nome do arquivo onde os resultados serão salvos.
- `sizes=("500" "1000" "2000")`: Tamanhos das matrizes a serem testadas.
- `threads=("1" "2" "4" "8")`: Números de threads para a versão concorrente.

### 2. Geração e Execução de Testes

```bash
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
```

- **Geração de Matrizes**: Usa `./geraMatrizBinario` para criar matrizes de entrada com os tamanhos especificados.
- **Execução Sequencial**: Executa o programa de multiplicação sequencial `./mat_seq` e registra os tempos de execução.
- **Execução Concorrente**: Executa o programa de multiplicação concorrente `./mat_conc` com diferentes números de threads e registra os tempos de execução.

### 3. Arquivo de Saída

Os resultados são salvos no arquivo `results.txt`, que inclui:
- O tamanho da matriz.
- O tipo de execução (sequencial ou concorrente).
- O número de threads (para a versão concorrente).
- Os tempos de execução para cada teste.

## Requisitos

1. **Permissões de Execução**: Antes de executar o script `roda_testes.sh`, é necessário garantir que ele tenha permissões de execução. Você pode definir as permissões executando o seguinte comando:

   ```bash
   chmod +x roda_testes.sh
   ```

2. **Compilação dos Programas**: Os seguintes programas devem estar compilados e disponíveis no diretório onde o script será executado:

   - `geraMatrizBinario`
   - `mat_seq`
   - `mat_conc`

3. **Defina Permissões**: Torne o script `roda_testes.sh` executável:

   ```bash
   chmod +x roda_testes.sh
   ```

3. **Execute o Script**: Rode o script `roda_testes.sh` para iniciar o benchmark e gerar o arquivo de resultados.

   ```bash
   ./roda_testes.sh
   ```

4. **Verifique os Resultados**: Abra o arquivo `results.txt` para visualizar os tempos de execução e comparar o desempenho das diferentes implementações e configurações.
