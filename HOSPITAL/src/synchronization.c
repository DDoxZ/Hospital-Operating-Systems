/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include "../include/synchronization.h"

#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

sem_t * semaphore_create(char* name, int value) {
    sem_t *sem = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, value);

    if (sem == SEM_FAILED) {
        perror("Erro a criar semáforo");
        exit(1);
    }

    //sem_init(sem, 1, value);

    return sem;
}

void semaphore_destroy(char* name, sem_t* semaphore) {
    if (sem_close(semaphore) == -1) {
        perror("Erro a fechar semáforo");
        exit(1);
    }

    if (sem_unlink(name) == -1) {
        perror("Erro a remover semáforo");
        exit(1);
    }
}

void produce_begin(struct prodcons* pc) {
    semaphore_lock(pc->empty);
    semaphore_lock(pc->mutex);
}

void produce_end(struct prodcons* pc) {
    semaphore_unlock(pc->mutex);
    semaphore_unlock(pc->full);
}

void consume_begin(struct prodcons* pc) {
    semaphore_lock(pc->full);
    semaphore_lock(pc->mutex);
}

void consume_end(struct prodcons* pc) {
    semaphore_unlock(pc->mutex);
    semaphore_unlock(pc->empty);
}

void semaphore_lock(sem_t* sem) {
    if (sem_wait(sem) == -1) {
        perror("Erro a fazer wait ao semáforo");
        exit(1);
    }
}

void semaphore_unlock(sem_t* sem) {
    if (sem_post(sem) == -1) {
        perror("Erro a fazer post ao semáforo");
        exit(1);
    }
}
