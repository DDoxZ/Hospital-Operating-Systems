/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include "../include/patient.h"
#include "../include/hosptime.h"
#include <stdio.h>

int execute_patient(int patient_id, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    struct admission *ad;
    while(1) {
        semaphore_lock(sems->terminate_mutex);
        if (*data->terminate == 1) {
          semaphore_unlock(sems->terminate_mutex);
          return data->patient_stats[patient_id];
        }
        semaphore_unlock(sems->terminate_mutex);

        patient_receive_admission(ad, patient_id, data, comm, sems);
        
        if (ad->id != -1 && *data->terminate == 0) {
            printf("[Patient %d] Recebi a admissão %d!\n", patient_id, ad->id);

            patient_process_admission(ad, patient_id, data, sems);
            patient_send_admission(ad, data, comm, sems);
        } else {
            semaphore_lock(sems->main_patient->empty);
            semaphore_unlock(sems->main_patient->full);
        }
    }
}

void patient_receive_admission(struct admission* ad, int patient_id, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    semaphore_lock(sems->terminate_mutex);
    if (*data->terminate == 1) {
      semaphore_unlock(sems->terminate_mutex);
      return;
    }
    semaphore_unlock(sems->terminate_mutex);
    
    consume_begin(sems->main_patient);
    read_main_patient_buffer(comm->main_patient, patient_id, data->buffers_size, ad);
    consume_end(sems->main_patient);
}

void patient_process_admission(struct admission* ad, int patient_id, struct data_container* data, struct semaphores* sems) {
    ad->receiving_patient = patient_id;
    ad->status = 'P';
    get_patient_time(ad); //Temporização da admissão do lado do paciente

    semaphore_lock(sems->patient_stats_mutex);
    data->patient_stats[patient_id]++;
    semaphore_unlock(sems->patient_stats_mutex);

    semaphore_lock(sems->results_mutex);
    data->results[ad->id] = *ad;
    semaphore_unlock(sems->results_mutex);
}

void patient_send_admission(struct admission* ad, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    produce_begin(sems->patient_receptionist);
    write_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
    produce_end(sems->patient_receptionist);
}

