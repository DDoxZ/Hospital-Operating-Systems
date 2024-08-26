/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#ifndef HOSPTIME_H_GUARD
#define HOSPTIME_H_GUARD

#include "../include/memory.h"

#include <time.h>

/*
 * Função que guarda o tempo de criação do pedido na estrutura data_container
*/
void get_create_time(struct admission* ad);

/*
 * Função que guarda o tempo de recessão do pedido 
 * do lado do paciente na estrutura data_container
*/
void get_patient_time(struct admission* ad);

/*
 * Função que guarda o tempo de recessão do pedido 
 * do lado do recepcionista na estrutura data_container
*/
void get_receptionist_time(struct admission* ad);

/*
 * Função que guarda o tempo de recessão do pedido 
 * do lado do médico na estrutura data_container
*/
void get_doctor_time(struct admission* ad);

#endif