/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include "../include/stats.h"

static FILE* ficheiro;

static void print_statistics(struct data_container* data) {
  ficheiro = fopen(data->statistics_filename, "r");
  if (ficheiro == NULL) {
    perror("abrir ficheiro de estatisticas(r)");
    exit(1);
  }

  char line[1024];
  while(fgets(line, sizeof(line), ficheiro) != NULL) 
    printf("%s", line);
  
  fclose(ficheiro);
}

void process_statistics(struct data_container* data, int* ad_counter) {
  ficheiro = fopen(data->statistics_filename, "w");
  if (ficheiro == NULL) {
    perror("abrir ficheiro de estatisticas(w)");
    exit(1);
  }

  struct tm *time;

  fprintf(ficheiro, "Process Statistics:\n");
  for (int i = 0; i < data->n_patients; i++) 
    if (data->patient_stats[i] != 0) 
      fprintf(ficheiro, "Patient %d requested %d admissions!\n", i, data->patient_stats[i]);
  
  
  for (int i = 0; i < data->n_receptionists; i++) 
    if (data->receptionist_stats[i] != 0) 
      fprintf(ficheiro, "Receptionist %d forwarded %d admissions!\n", i, data->receptionist_stats[i]);
  
  for (int i = 0; i < data->n_doctors; i++) 
    if (data->doctor_stats[i] != 0) 
      fprintf(ficheiro, "Doctor %d processed %d admissions!\n", i, data->doctor_stats[i]);
  

  fprintf(ficheiro, "\nAdmission Statistics:\n");

  for (int ad = 0; ad < *ad_counter; ad++) {
    fprintf(ficheiro, "Admission: %d\n", data->results[ad].id);
    fprintf(ficheiro, "Status: %c\n", data->results[ad].status);
    fprintf(ficheiro, "Patient id: %d\n", data->results[ad].receiving_patient);
    fprintf(ficheiro, "Receptionist id: %d\n", data->results[ad].receiving_receptionist);
    fprintf(ficheiro, "Doctor id: %d\n", data->results[ad].receiving_doctor);

    // Fazer em funcao auxiliar depois para simplificar
    time = localtime(&data->results[ad].create_time.tv_sec);
    fprintf(ficheiro, "Start time: %02d/%02d/%d_%02d:%02d:%02d.%03ld\n", time->tm_mday, time->tm_mon + 1, 
    time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec, data->results[ad].create_time.tv_nsec / 1000000);

    time = localtime(&data->results[ad].patient_time.tv_sec);
    fprintf(ficheiro, "Patient time: %02d/%02d/%d_%02d:%02d:%02d.%03ld\n", time->tm_mday, time->tm_mon + 1, 
    time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec, data->results[ad].create_time.tv_nsec / 1000000);

    time = localtime(&data->results[ad].receptionist_time.tv_sec);
    fprintf(ficheiro, "Patient time: %02d/%02d/%d_%02d:%02d:%02d.%03ld\n", time->tm_mday, time->tm_mon + 1, 
    time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec, data->results[ad].create_time.tv_nsec / 1000000);

    time = localtime(&data->results[ad].doctor_time.tv_sec);
    fprintf(ficheiro, "Patient time: %02d/%02d/%d_%02d:%02d:%02d.%03ld\n", time->tm_mday, time->tm_mon + 1, 
    time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec, data->results[ad].create_time.tv_nsec / 1000000);

    fprintf(ficheiro, "Total Time: %ld.%03ld", data->results[ad].doctor_time.tv_sec - data->results[ad].create_time.tv_sec, 
    data->results[ad].doctor_time.tv_nsec - data->results[ad].create_time.tv_nsec / 1000000);

    fprintf(ficheiro, "\n");
  }
  
  fclose(ficheiro); 
}