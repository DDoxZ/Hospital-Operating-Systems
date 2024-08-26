/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#ifndef LOG_H_GUARD
#define LOG_H_GUARD

/* Função que apaga um ficheiro de log
*/
void delete_log(char* filename);

/* Função que escreve log de operacoes para um ficheiro
*/
void write_log(char* filename, char* operation, int arguments[]);

#endif