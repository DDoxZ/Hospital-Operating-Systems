/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include "../include/hosptime.h"
#include "../include/memory.h"
#include <stdio.h>
#include <stdlib.h>

static struct timespec currentTime;

static void get_time() {
    if (clock_gettime(CLOCK_REALTIME, &currentTime) == -1) {
        perror("clock_gettime");
        exit(1);
    }
}

void get_create_time(struct admission* ad){
    get_time();
    ad->create_time = currentTime;
}

void get_patient_time(struct admission* ad){
    get_time();
    ad->patient_time = currentTime;
}

void get_receptionist_time(struct admission* ad){
    get_time();
    ad->receptionist_time = currentTime;
}

void get_doctor_time(struct admission* ad){
    get_time();
    ad->doctor_time = currentTime;
}
