/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include "../include/doctor.h"
#include "../include/hosptime.h"
#include "../include/log.h"
#include <stdio.h>

int execute_doctor(int doctor_id, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    struct admission *ad;
    while(1) {
        semaphore_lock(sems->terminate_mutex);
        if (*data->terminate == 1) {
          semaphore_unlock(sems->terminate_mutex);
          return data->doctor_stats[doctor_id];
        }
        semaphore_unlock(sems->terminate_mutex);

        doctor_receive_admission(ad, doctor_id, data, comm, sems);

        if(ad->id != -1 && *data->terminate == 0) {
            printf("[Doctor %d] Recebi a admissão %d!\n", doctor_id, ad->id);

            doctor_process_admission(ad, doctor_id, data, sems);
        } else {
            semaphore_lock(sems->receptionist_doctor->empty);
            semaphore_unlock(sems->receptionist_doctor->full);
        }
    }
}

void doctor_receive_admission(struct admission* ad, int doctor_id, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    semaphore_lock(sems->terminate_mutex);
    if (*data->terminate == 1) {
      semaphore_unlock(sems->terminate_mutex);
      return;
    }
    semaphore_unlock(sems->terminate_mutex);

    consume_begin(sems->receptionist_doctor);
    read_receptionist_doctor_buffer(comm->receptionist_doctor, doctor_id, data->buffers_size, ad);
    consume_end(sems->receptionist_doctor);
}

void doctor_process_admission(struct admission* ad, int doctor_id, struct data_container* data, struct semaphores* sems) {
    int n_admissions = ad->id + 1;

    if (n_admissions <= data->max_ads) {
        ad->receiving_doctor = doctor_id;
        ad->status = 'A';
    } else {
        ad->status = 'N';
    }

    get_doctor_time(ad); //Temporização da admissão do lado do médico

    semaphore_lock(sems->doctor_stats_mutex);
    data->doctor_stats[doctor_id]++;
    semaphore_unlock(sems->doctor_stats_mutex);

    semaphore_lock(sems->results_mutex);
    data->results[ad->id] = *ad;
    semaphore_unlock(sems->results_mutex);
}