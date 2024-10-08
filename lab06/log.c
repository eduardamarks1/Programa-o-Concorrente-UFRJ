#include "log.h"
#include <stdlib.h>
#include <string.h>

static FILE* file = NULL;

pthread_mutex_t log_mutex;

void LOG_init(const char* filename) {
    pthread_mutex_init(&log_mutex, NULL); //inicializa o mutex que vai fazer a exclusão mútua para o arquivo
    file = fopen(filename, "a"); // abre o arquivo em modo de anexação
    if (file == NULL) printf("Erro ao abrir arquivo");; 
}

void LOG_close() {
    fclose(file);
    pthread_mutex_destroy(&log_mutex);
}

void LOG(const char* message) {
    pthread_mutex_lock(&log_mutex);
    if (file != NULL) {
        fprintf(file, "[LOG] %s\n", message);
        fflush(file);
    }
    pthread_mutex_unlock(&log_mutex);
}
