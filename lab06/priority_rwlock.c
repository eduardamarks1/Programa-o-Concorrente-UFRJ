#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "priority_rwlock.h"
#include "LOG.h" 

void rwlock_priority_init() {
    escr = 0;
    leit = 0;
    escrEsperando = 0; 
    pthread_cond_init(&condEscritores, NULL);
    pthread_cond_init(&condLeitores, NULL);
    LOG_init("LOG.txt");
}

void rwlock_priority_destroy() {
    LOG_close(); 
    pthread_cond_destroy(&condEscritores);
    pthread_cond_destroy(&condLeitores);
}

// entra na seção de leitura 
void rwlock_priority_read_lock(pthread_mutex_t* mutex) {
    pthread_mutex_lock(mutex);
    LOG("Leitor tenta ler");

    while (escr > 0 || escrEsperando > 0) {
        LOG("Leitor esperando, porque tem escritor ativo ou esperando");
        pthread_cond_wait(&condLeitores, mutex);
    }
    leit++;
    LOG("Leitor comecou a ler");
    pthread_mutex_unlock(mutex);
}

// sai da seção de leitura
void rwlock_priority_read_unlock(pthread_mutex_t* mutex) {
    pthread_mutex_lock(mutex);
    leit--;
    LOG("Leitor terminou de ler");
    if (leit == 0) pthread_cond_signal(&condEscritores); // acorda escritor
    pthread_mutex_unlock(mutex);
}

// entra na seção de escrita (COM PRIORIDADE)
void rwlock_priority_write_lock(pthread_mutex_t* mutex) {
    pthread_mutex_lock(mutex);
    escrEsperando++;
    LOG("Escritor tenta escrever");

    while (escr > 0 || leit > 0) {
        LOG("Escritor esperando, porque tem leitores ou escritores ativos");
        pthread_cond_wait(&condEscritores, mutex);
    }
    escrEsperando--; 
    escr++;
    LOG("Escritor comecou a escrever");
    pthread_mutex_unlock(mutex);
}

// sai da seção de escrita
void rwlock_priority_write_unlock(pthread_mutex_t* mutex) {
    pthread_mutex_lock(mutex);
    escr--;
    LOG("Escritor terminou de escrever");
    
    if (escr == 0) {
        if (escrEsperando > 0) {
            pthread_cond_signal(&condEscritores); // prioriza escritores
        } else {
            pthread_cond_broadcast(&condLeitores); 
        }
    }
    pthread_mutex_unlock(mutex);
}
