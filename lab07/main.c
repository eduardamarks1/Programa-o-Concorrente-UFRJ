#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// fluxo principal
int main(int argc, char *argv[]) {
    int N;
    pthread_t *tid;
    char *buffer1, *buffer2;

    if (argc < 3) {
        printf("Uso: %s <número de threads> <arquivo de entrada>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);

    tid = (pthread_t*) malloc(sizeof(pthread_t) * N);
    if (tid == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para as threads (malloc).\n");
        return 2;
    }

    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: não foi possível abrir o arquivo %s.\n", argv[2]);
        return 1;
    }

    buffer1 = (char *)malloc(sizeof(char) * N);
    if (buffer1 == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para buffer1 (malloc).\n");
        fclose(file);
        free(tid);
        return 1;
    }

    buffer2 = (char *)malloc(sizeof(char) * N);
    if (buffer2 == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para buffer2 (malloc).\n");
        fclose(file);
        free(buffer1);
        free(tid);
        return 1;
    }

    if (pthread_create(&tid[0], NULL, t1, NULL)) {
        fprintf(stderr, "Erro: falha ao criar a thread 1 (pthread_create).\n");
        exit(-1);
    }
    if (pthread_create(&tid[1], NULL, t2, NULL)) {
        fprintf(stderr, "Erro: falha ao criar a thread 2 (pthread_create).\n");
        exit(-1);
    }
    if (pthread_create(&tid[2], NULL, t3, NULL)) {
        fprintf(stderr, "Erro: falha ao criar a thread 3 (pthread_create).\n");
        exit(-1);
    }

    for (int t = 0; t < N; t++) {
        if (pthread_join(tid[t], NULL)) {
            fprintf(stderr, "Erro: falha ao esperar a thread %d (pthread_join).\n", t);
            exit(-1);
        }
    }

    fclose(file);
    free(buffer1);
    free(buffer2);
    free(tid);

    return 0;
}
