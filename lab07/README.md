
## Processamento de arquivo usando semáforo

### Descrição
Este programa em C lê um arquivo de entrada (`entrada.txt`), processa seus caracteres usando 2 buffers e 3 threads, e escreve o resultado no arquivo de saída (`saida.txt`). Ele utiliza **pthread** e **semaforos** para sincronizar o acesso entre as três threads:

- **Thread 1**: Lê o arquivo de entrada e armazena os caracteres no `buffer1`.
- **Thread 2**: Processa os dados do `buffer1` e os transfere para o `buffer2`, inserindo um caracter de final de linha a cada 2n + 1 caracteres lidos.
- **Thread 3**: Escreve os dados do `buffer2` no arquivo de saída.

### Como Rodar
1. Compile o código com o seguinte comando:
   ```bash
   gcc -o main main.c -lpthread
   ```

2. Crie o arquivo de entrada `entrada.txt` com o conteúdo que deseja processar. (Por padrão está o `entrada.txt`)

3. Execute o programa passando o tamanho do buffer como argumento:
   ```bash
   ./main <tamanho_do_buffer>
   ```

   **Exemplo**:
   ```bash
   ./main 10
   ```

Este programa demonstra o uso eficiente de **threads e semáforos** para processamento concorrente de arquivos.
