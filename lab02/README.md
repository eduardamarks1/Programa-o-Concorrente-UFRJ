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

   e = |(vs - vc) / vs|


  Onde:
  - \(v_s\): valor do programa sequencial
  - \(v_c\): valor do programa concorrente

### Conclusão

Ao testar o código gerando vetores de dimensão 10000  e 100 threads obteve-se como saída:

pi_seq (invertida)         = 277368416.00000000000000000000000000

pi_seq_blocos (2 blocos)   = 277368512.00000000000000000000000000

pi_concorrente             = 277368480.00000000000000000000000000

Produto_Interno_Original                   = 277368525.46223115921020507812500000

Variacao Relativa p/ Partição Sequencial Invertida      = 0.00000039464546830458857585

Variacao Relativa p/ Partição em blocos sequenciais       = 0.00000004853554003148019547

Variacao Relativa p/ Partição com Threads       = 0.00000016390551138556475053

- Com esse teste e outro com dimensões mais baixas, foi possível perceber que à medida que o número de elementos e o número de threads aumentam, as diferenças na precisão entre os métodos se tornam mais perceptíveis. A execução concorrente, mesmo com vetores de maiores dimensões ainda apresenta um pequeno erro em relação ao original, mas ainda manteve uma precisão razoável. O desvio mais significativo ocorre na abordagem sequencial invertida, porque a ordem de soma dos produtos internos pode impactar a precisão, especialmente nos vetores grandes, devido à forma como os erros de ponto flutuante se acumulam na máquina. É importante mencionar que foi tilizado o tipo `float` para armazenar as variáveis que calculam as variações relativas porque, ao usar o tipo `double`, os valores resultantes eram todos zero, devido à precisão excessiva que "mascarava" as pequenas diferenças entre os resultados e o valor original do produto interno armazenado no arquivo pelo programa sequencial. Esse ajuste permitiu que as variações fossem de fato detectadas, o que relembrou os conceitos estudados na disciplina de Cálculo Numérico, especialmente em relação à propagação de erros nas operações e à precisão na representação por ponto flutuante. 

- Ao realizar o teste com uma dimensão de 100.000 elementos e utilizando 1.000 threads, a maior imprecisão foi encontrada na partição sequencial invertida, enquanto a por blocos sequenciais foi a que apresentou a maior proximidade com o valor original. Tentei aumentar o número de threads para ver se a versão concorrente ganhava, mas isso não aconteceu.
