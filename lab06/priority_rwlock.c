#include <pthread.h>
#include <stdio.h>

pthread_cond_t condLeitores, condEscritores;
int leit = 0, escr = 0;

void rwlock_priority_init(){
    pthread_cond_init(&condEscritores, NULL);
    pthread_cond_init(&condLeitores, NULL);
}

void rwlock_priority_destroy(){
    pthread_cond_destroy(&condEscritores);
    pthread_cond_destroy(&condLeitores);
}

// entra na seção de leitura 
void rwlock_priority_read_lock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    while(escr > 0){ 
        pthread_cond_wait(&condEscritores, mutex);
    }
    leit++;
    pthread_mutex_unlock(mutex);
}

// sai da seção de leitura 
void rwlock_priority_read_unlock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    leit--;
    if(leit == 0) pthread_cond_signal(&condEscritores);
    pthread_mutex_unlock(mutex);
}

// entra na seção de escrita com prioridade
void rwlock_priority_write_lock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    while(escr > 0 || leit > 0){
        pthread_cond_wait(&condEscritores, mutex);
    }
    escr++;
    pthread_mutex_unlock(mutex);
}

// sai da seção de escrita com prioridade
void rwlock_priority_write_unlock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    escr--;
    if(escr == 0) pthread_cond_signal(&condEscritores);
    pthread_cond_broadcast(&condEscritores);
    pthread_mutex_unlock(mutex);
}
