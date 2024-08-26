/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include "../include/configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int read_config_file(char* filename, struct data_container* data) {
    FILE* config_file = fopen(filename, "r");
    if (config_file == NULL) {
        printf("[Main] Error: Could not open file %s\n", filename);
        return -1;
    }

    // Read each line directly and assign values to corresponding struct fields
    if (fscanf(config_file, "%d\n", &data->max_ads) != 1) {
        printf("[Main] Error: Failed to read max_ads from file\n");
        fclose(config_file);
        return -1;
    }
    if (data->max_ads <= 0) {
        printf("[Main] Error: max_ads must be a positive integer\n");
        fclose(config_file);
        return -1;
    }
    //printf("max_ads: %d\n", data->max_ads);

    if (fscanf(config_file, "%d\n", &data->buffers_size) != 1) {
        printf("[Main] Error: Failed to read buffers_size from file\n");
        fclose(config_file);
        return -1;
    }
    if (data->buffers_size <= 0) {
        printf("[Main] Error: buffers_size must be a positive integer\n");
        fclose(config_file);
        return -1;
    }
    //printf("buffers_size: %d\n", data->buffers_size);

    if (fscanf(config_file, "%d\n", &data->n_patients) != 1) {
        printf("[Main] Error: Failed to read n_patients from file\n");
        fclose(config_file);
        return -1;
    }

    if (data->n_patients <= 0) {
        printf("[Main] Error: n_patients must be a positive integer\n");
        fclose(config_file);
        return -1;
    }
    //printf("n_patients: %d\n", data->n_patients);

    if (fscanf(config_file, "%d\n", &data->n_receptionists) != 1) {
        printf("[Main] Error: Failed to read n_receptionists from file\n");
        fclose(config_file);
        return -1;
    }
    if (data->n_receptionists <= 0) {
        printf("[Main] Error: n_receptionists must be a positive integer\n");
        fclose(config_file);
        return -1;
    }
    //printf("n_receptionists: %d\n", data->n_receptionists);

    if (fscanf(config_file, "%d\n", &data->n_doctors) != 1) {
        printf("[Main] Error: Failed to read n_doctors from file\n");
        fclose(config_file);
        return -1;
    }
    if (data->n_doctors <= 0) {
        printf("[Main] Error: n_doctors must be a positive integer\n");
        fclose(config_file);
        return -1;
    }
    //printf("n_doctors: %d\n", data->n_doctors);

    if (fscanf(config_file, "%s\n", data->log_filename) != 1) {
        printf("[Main] Error: Failed to read log_filename from file\n");
        fclose(config_file);
        return -1;
    }
    //printf("log_filename: %s\n", data->log_filename);

    if (fscanf(config_file, "%s\n", data->statistics_filename) != 1) {
        printf("[Main] Error: Failed to read statistics_filename from file\n");
        fclose(config_file);
        return -1;
    }
    //printf("statistics_filename: %s\n", data->statistics_filename);

    if (fscanf(config_file, "%d\n", &data->alarm_time) != 1) {
        printf("[Main] Error: Failed to read alarm_time from file\n");
        fclose(config_file);
        return -1;
    }
    //printf("alarm_time: %d\n", data->alarm_time);

    fclose(config_file);

    return 0;
}