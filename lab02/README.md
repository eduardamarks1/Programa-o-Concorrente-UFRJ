# Lab02 - Programação Concorrente


### Cálculo do Produto Interno de Vetores  (Sequencial e Concorrente)

Este projeto consiste na implementação de dois programas para calcular o produto interno de dois vetores utilizando abordagens sequenciais e concorrentes, com o objetivo de comparar o desempenho e a precisão entre essas abordagens.

### Estrutura do Projeto

O projeto contém os seguintes componentes:

**Gerador de Vetores Aleatórios (`gera_2vet_rand.c`)**: Este programa gera dois vetores de números aleatórios do tipo `float` com dimensão `N` e os salva em um arquivo binário. O programa também calcula o produto interno, de forma sequencial, desses dois vetores e armazena o resultado no mesmo arquivo.

**Programa Concorrente (`p_interno_conc.c`)**: Este programa realiza o cálculo do produto interno de dois vetores utilizando múltiplas threads para dividir a tarefa de forma balanceada. Ele recebe como entrada o número de threads `T`, o nome do arquivo que contém os vetores e realiza o cálculo, comparando o resultado com o valor registrado no arquivo. Além disso, para fins de comparação, além da partição utilizando threads, tendo como base o código disponível para esta tarefa, foram calculadas também os produtos internos de forma sequência (invertida) e separando em dois blocos.

### Como Executar

Utilize o programa `gera_2vet_rand.c` para gerar arquivos binários contendo os vetores de teste.

   ```bash
   gcc -o gera_2vet_rand gera_2vet_rand.c
   ./gera_2vet_rand <dimensao> <arquivo_saida>
   ```

   Exemplo: `./gera_2vet_rand 100 vetores.bin`


**Executar o Programa Concorrente**:

   ```bash
    gcc -o p_interno_conc p_interno_conc.c -pthread
   ./p_interno_conc <arquivo_entrada> <numero_threads>
   ```

   Exemplo: `./p_interno_conc vetores.bin 50`

### Resultados

- Após a execução dos programas, os resultados foram comparados utilizando o cálculo da variação relativa entre os métodos sequencial e concorrente a partir da fórmula:

  \[
  e = \left| \frac{v_s - v_c}{v_s} \right|
  \]

  Onde:
  - \(v_s\): valor do programa sequencial
  - \(v_c\): valor do programa concorrente

### Conclusão

