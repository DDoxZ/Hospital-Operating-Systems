/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#ifndef STATS_H_GUARD
#define STATS_H_GUARD

#include "../include/main.h"

/* Função que escreve as estatisticas finais do hospital para o ficheiro 
* de nome log_filename em data_container e para o terminal.
*/
void process_statistics(struct data_container* data, int* ad_counter);

#endif