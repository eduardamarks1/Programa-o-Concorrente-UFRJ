#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

char *buffer1;
char *buffer2;
int N;  // tamanho do buffer1 e dos blocos
char *ptr_leitura;   // ponteiro de leitura no buffer1
char *ptr_escrita;   // ponteiro de escrita no buffer2
int leitura_ativa = 1;  // indica se ainda há dados para processar
int buffer1_vazio = 1;  // indica se o buffer1 está vazio

sem_t sem_buffer1, sem_buffer2;  // controle de acesso exclusivo aos buffers
sem_t buffer1_pronto, buffer2_pronto;  // controle dos cs disponíveis

// lê os caracteres do arquivo de entrada e deposita no buffer1
void *t1(void *arg) {
    FILE *entrada = fopen("entrada.txt", "r");
    if (entrada == NULL) {
        printf("erro ao abrir o arquivo de entrada.\n");
        pthread_exit(NULL);
    }

    while (1) {
        sem_wait(&sem_buffer1);  // acesso exclusivo ao buffer1

        ptr_leitura = buffer1;  // aponta para o início do buffer1
        buffer1_vazio = 0;      // buffer1 agora tem dados

        // lê até o tamanho do buffer ou o fim do arquivo
        for (int i = 0; i < N; i++) {
            int c = fgetc(entrada); // pega o caractere

            if (c == EOF) {  // fim do arquivo
                leitura_ativa = 0;
                break;
            }

            *ptr_leitura++ = c;  // armazena no buffer1
        }
        *ptr_leitura = '\0';  // finaliza o buffer1 com caractere nulo

        sem_post(&buffer1_pronto);  // sinaliza que o buffer1 está pronto

        if (leitura_ativa == 0) {  // termina ao fim do arquivo
            fclose(entrada);
            pthread_exit(NULL);
        }
    }
}

// processa os caracteres do buffer1 e deposita no buffer2
void *t2(void *arg) {
    int cont_c = 0;
    int cont_linha = 0;

    while (1) {
        sem_wait(&buffer1_pronto);   // espera o buffer1 estar pronto
        sem_wait(&sem_buffer2);      // acesso exclusivo ao buffer2

        ptr_escrita = buffer2;  // ponteiro de escrita volta ao início

        // copia caracteres do buffer1 para o buffer2
        for (char *ptr = buffer1; *ptr != '\0'; ptr++) {
            *ptr_escrita++ = *ptr;  // copia caractere do buffer1 para buffer2
            cont_c++;

            // verifica se deve inserir '\n'
            int inserir_nova_linha =
                (cont_linha >= (N + 1) && cont_c == N) ||
                (cont_c == 2 * cont_linha + 1);

            if (inserir_nova_linha) {
                *ptr_escrita++ = '\n';  // insere nova linha no buffer2
                if (cont_linha < (N + 1)) cont_linha++;
                cont_c = 0;
            }
        }
        *ptr_escrita = '\0';  // finaliza o buffer2 com caractere nulo

        buffer1_vazio = 1;  // buffer1 foi esvaziado

        sem_post(&buffer2_pronto);  // sinaliza que o buffer2 está pronto
        sem_post(&sem_buffer1);     // libera o buffer1

        if (leitura_ativa == 0 && buffer1_vazio) { 
            pthread_exit(NULL);
        }
    }
}

// escreve os caracteres do buffer2 no arquivo de saída
void *t3(void *arg) {
    FILE *saida = fopen("saida.txt", "w");
    if (saida == NULL) {
        printf("erro ao abrir o arquivo de saída.\n");
        pthread_exit(NULL);
    }

    while (1) {
        sem_wait(&buffer2_pronto);  // espera o buffer2 estar pronto

        // escreve no arquivo até encontrar o caractere nulo
        for (char *ptr = buffer2; *ptr != '\0'; ptr++) {
            fputc(*ptr, saida);
        }
        fflush(saida);  // garante que os dados sejam gravados imediatamente

        sem_post(&sem_buffer2);  // libera o buffer2

        if (leitura_ativa == 0 && buffer1_vazio) {  // fim do processamento
            fclose(saida);
            pthread_exit(NULL);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("uso: %s <tamanho_do_buffer>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);

    if (N <= 0) {
        printf("erro: o tamanho do buffer deve ser maior que 0.\n");
        return 1;
    }

    // aloca memória para os buffers (adicionando espaço para o nulo)
    buffer1 = (char *)malloc((N + 1) * sizeof(char));
    buffer2 = (char *)malloc((N + 1) * sizeof(char));

    if (buffer1 == NULL || buffer2 == NULL) {
        printf("erro ao alocar memória para os buffers.\n");
        return 1;
    }

    // inicializa os semáforos binários
    sem_init(&sem_buffer1, 0, 1);
    sem_init(&buffer1_pronto, 0, 0);
    sem_init(&sem_buffer2, 0, 1);
    sem_init(&buffer2_pronto, 0, 0);

    // cria as threads
    pthread_t id_threads[3];
    pthread_create(&id_threads[0], NULL, t1, NULL);
    pthread_create(&id_threads[1], NULL, t2, NULL);
    pthread_create(&id_threads[2], NULL, t3, NULL);

    // aguarda as threads terminarem
    pthread_join(id_threads[0], NULL);
    pthread_join(id_threads[1], NULL);
    pthread_join(id_threads[2], NULL);

    // libera a memória alocada para os buffers
    free(buffer1);
    free(buffer2);

    // destroi os semáforos
    sem_destroy(&sem_buffer1);
    sem_destroy(&buffer1_pronto);
    sem_destroy(&sem_buffer2);
    sem_destroy(&buffer2_pronto);

    return 0;
}
