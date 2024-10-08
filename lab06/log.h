#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <pthread.h>

void LOG_init(const char* filename);
void LOG_close();
void LOG(const char* message);
extern pthread_mutex_t log_mutex;

#endif
