/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include "../include/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void delete_log(char* filename) {
  remove(filename);
}

void write_log(char* filename, char* operation, int arguments[]){
  FILE* file;
  file = fopen(filename, "a");
  if ( file == NULL) {
    perror("erro ao abrir ficheiro de log");
    exit(1);
  }

  struct timespec currentTime;
  if(clock_gettime(CLOCK_REALTIME, &currentTime) == -1){
    perror("erro ao obter tempo atual");
    exit(1);
  }

  struct tm* time_info;
  time_info = localtime(&currentTime.tv_sec);

  if(strcmp(operation, "ad") == 0){
    fprintf(file, "%02d/%02d/%d_%02d:%02d:%02d.%03ld %s %d %d\n", time_info->tm_mday, time_info->tm_mon + 1, 
            time_info->tm_year + 1900, time_info->tm_hour, time_info->tm_min, time_info->tm_sec, currentTime.tv_nsec/1000000, operation, arguments[0], arguments[1]);
  }else if (strcmp(operation, "info") == 0) {
    fprintf(file, "%02d/%02d/%d_%02d:%02d:%02d.%03ld %s %d\n", time_info->tm_mday, time_info->tm_mon + 1, 
            time_info->tm_year + 1900, time_info->tm_hour, time_info->tm_min, time_info->tm_sec, currentTime.tv_nsec/1000000, operation, arguments[0]);
  } else {
    fprintf(file, "%02d/%02d/%d_%02d:%02d:%02d.%03ld %s\n", time_info->tm_mday, time_info->tm_mon + 1, 
          time_info->tm_year + 1900, time_info->tm_hour, time_info->tm_min, time_info->tm_sec, currentTime.tv_nsec/1000000, operation);
  }

  fclose(file);
}
