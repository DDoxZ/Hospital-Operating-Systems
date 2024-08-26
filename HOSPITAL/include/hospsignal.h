/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#ifndef HOSPSIGNAL_H_GUARD
#define HOSPSIGNAL_H_GUARD

#include "main.h"

/* Função que inicia os sinais para escrever as admissões existentes em data 
* com alarm_time de intervalo de tempo, e que fica a escuta de um crtl+c para
* encerrar o hospital.
*/
void launch_signals(int alarm_time, int* ad_counter, struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Função que escreve as admissões existentes em data */
static void write_admissions();

#endif
