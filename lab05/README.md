# Lab05 - Sincronização de Threads para Imprimir Múltiplos de 10

## Descrição

Este programa em C cria um sistema multithreading que imprime os primeiros 20 múltiplos de 10, utilizando dois tipos de threads: uma chamada `ExecutaTarefa`, que faz o incremento de um valor, e uma thread de log chamada `extra`, que é responsável por imprimir os múltiplos de 10 encontrados. A sincronização entre as threads é feita através de mutex e variáveis de condição para garantir que a thread de log imprima o valor antes que a thread de incremento continue.

## Lógica de Sincronização

1. **Thread `ExecutaTarefa`:**
   - Esta thread incrementa a variável `soma` a cada ciclo.
   - Quando a variável `soma` atinge um múltiplo de 10, a thread pausa e aguarda que a thread `extra` faça a impressão do valor.
   - A execução só é retomada após o múltiplo de 10 ser impresso, evitando que múltiplos de 10 sejam "pulados" ou impressos fora de ordem.
  
2. **Thread `extra`:**
   - Essa thread monitora quando a variável `soma` atinge um múltiplo de 10 e imprime o valor correspondente.
   - Após a impressão, sinaliza as threads `ExecutaTarefa` para que continuem o incremento.
   - Isso garante que cada múltiplo de 10 seja impresso exatamente uma vez, de forma sincronizada com o progresso da soma.

A sincronização entre as threads é realizada com um `pthread_mutex_t` para controle do acesso à variável compartilhada `soma` e variáveis de condição (`pthread_cond_t`) para pausar e retomar a execução das threads de forma coordenada.

## Como Rodar o Código

1. **Compilação:**

   Use o `gcc` para compilar o programa:

   ```bash
   gcc -o soma-lock-sync soma-lock-sinc.c -Wall -pthread

      ```bash
   ./soma-lock-sync <quantidade de threads>
