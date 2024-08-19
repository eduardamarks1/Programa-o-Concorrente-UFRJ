# Lab 01 - Programação Concorrente

## Objetivo

Esta pasta contém a implementação do meu primeiro programa concorrente!

Esse programa concorrente utiliza M threads para somar 1 a cada elemento de um vetor de N elementos do tipo inteiro. Para cada elemento `a[i]` do vetor, o programa calcula o novo valor (`a[i] + 1`) e escreve o resultado na mesma posição do elemento. A tarefa completa é dividida entre as M threads de forma mais balanceada possível, garantindo que as threads recebam a mesma carga de trabalho. Os valores de M e N são informados na chamada do programa.

## Código

### Divisão de Tarefas entre as M Threads

- **Como foi dividida a tarefa?**  
  É calculado quantos elementos cada thread deverá processar (`elementosPorThread = N / M`). Caso o vetor não seja divisível pelo número de threads (`resto = N % M`), as primeiras `resto` threads recebem um elemento extra para processar, garantindo uma divisão o mais balanceada possível.

- **Como as threads executam a função?**  
  Todas as threads executam a mesma função, chamada `soma_um`, que é a função principal executada por cada thread. Ela recebe como argumento a estrutura `t_Args`, que contém as informações necessárias para a execução.

### Argumentos Passados para a Função Executada pelas Threads

- **`t_Args`:**  
  A função `soma_um` recebe um ponteiro para uma estrutura `t_Args` que contém:
  - `idThread`: ID da thread.
  - `M`: Quantidade total de threads.
  - `inicio`: Índice inicial do vetor que a thread deve incrementar.
  - `fim`: Índice final do vetor que a thread deve incrementar.
  - `vetor`: Ponteiro para o vetor que será modificado.

### Funções Adicionais

- **Função `preencherVetor`:**  
  Inicializa o vetor com valores aleatórios. 

- **Função `printaVetor`:**  
  Exibe os valores do vetor no serial. 

### Função para Verificar se o Resultado Final Está Correto

- **Função `verificarIncremento`:**  
  A função `verificarIncremento` compara o vetor original (`vetorOriginal`) com o vetor incrementado (`vetor`) para verificar se cada posição foi corretamente incrementada em 1. Ela percorre ambos os vetores, comparando elemento por elemento. Se houver alguma diferença entre elas, uma mensagem de erro é impressa, indicando a posição onde o erro ocorreu. Caso contrário, uma mensagem dizendo que todas as posições foram incrementadas corretamente é exibida.

## Comentários e Problemas Encontrados

- **Alocação Dinâmica de Memória:**  
  Enfrentei muitos problemas na alocação dinâmica do vetor, dado que o tamanho seria passado pela chamada do programa. Para resolver esses problemas, precisei relembrar as aulas de Computação 1.
  
- **Manipulação de Ponteiros:**  
  A manipulação de ponteiros nas funções `printaVetor` e `preencherVetor` também foi um problema. Pois, inicialmente, tive dificuldades para passar o vetor por referência e garantir que as funções fossem capazes de modificar o vetor adequadamente.

- **Distribuição de Tarefas:**  
  Gastei bastante tempo pensando em como distribuir as tarefas entre as threads. A solução encontrada como o cálculo do índice de início e fim para cada thread, foi a mais razoável. Tentei implementar uma abordagem sem o ponteiro para `fim`, mas isso exigiria cálculos adicionais e tornaria o código mais complexo, então acabei deixando dessa formaque talvez não seja a melhor.

- **Comparação entre Vetores:**  
  Não encontrei uma forma mais eficiente de comparar os vetores para verificar o resultado final. A abordagem atual é funcional, mas talvez haja maneiras mais elegantes de fazer essa comparação.

- **Medição do Tempo de Execução:**  
  Fiquei curiosa sobre como o tempo de execução varia com diferentes quantidades de threads. Ainda não implementei uma função para medir o tempo de execução, mas isso é algo que pretendo explorar em futuros experimentos para entender melhor o impacto do número de threads no desempenho do programa.

