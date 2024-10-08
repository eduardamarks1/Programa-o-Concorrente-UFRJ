#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG(mensagem) printf("[LOG] %s.\n", mensagem)

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
    LOG("Leitor tenta ler");
    while(escr > 0){
        LOG("Leitor esperando, porque tem um escritor ativo");
        pthread_cond_wait(&condEscritores, mutex);
    }
    leit++;
    LOG("Leitor comecou a ler");
    pthread_mutex_unlock(mutex);
}

// sai da seção de leitura 
void rwlock_priority_read_unlock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    leit--;
    LOG("Leitor terminou de ler");
    if(leit == 0) pthread_cond_signal(&condEscritores);
    pthread_mutex_unlock(mutex);
}

// entra na seção de escrita com prioridade
void rwlock_priority_write_lock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    LOG("Escritor tenta escrever");
    while(escr > 0 || leit > 0){
        LOG("Escritor esperando, porque tem leitores ou escritores ativos");
        pthread_cond_wait(&condEscritores, mutex);
    }
    escr++;
    LOG("Escritor comecou a escrever");
    pthread_mutex_unlock(mutex);
}

// sai da seção de escrita com prioridade
void rwlock_priority_write_unlock(pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    escr--;
    LOG("Escritor terminou de escrever");
    if(escr == 0) pthread_cond_signal(&condEscritores);
    pthread_cond_broadcast(&condEscritores);
    pthread_mutex_unlock(mutex);
}
