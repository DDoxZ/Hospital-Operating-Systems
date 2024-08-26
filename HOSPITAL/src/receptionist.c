/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/


#include "../include/receptionist.h"
#include "../include/hosptime.h"
#include <stdio.h>


int execute_receptionist(int receptionist_id, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    struct admission *ad;
    while(1){
        semaphore_lock(sems->terminate_mutex);
        if(*data->terminate == 1) {
            semaphore_unlock(sems->terminate_mutex);
            return data->receptionist_stats[receptionist_id];
        }
        semaphore_unlock(sems->terminate_mutex);

        receptionist_receive_admission(ad, data, comm, sems);

        if(ad->id != -1 && *data->terminate == 0){
            printf("[Receptionist %d] Recebi a admissão %d!\n", receptionist_id, ad->id);

            receptionist_process_admission(ad, receptionist_id, data, sems);
            receptionist_send_admission(ad, data, comm, sems);
        } else {
            semaphore_lock(sems->patient_receptionist->empty);
            semaphore_unlock(sems->patient_receptionist->full);
        }
    }
}


void receptionist_receive_admission(struct admission* ad, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    semaphore_lock(sems->terminate_mutex);
    if (*data->terminate == 1) {
      semaphore_unlock(sems->terminate_mutex);
      return;
    }
    semaphore_unlock(sems->terminate_mutex);

    consume_begin(sems->patient_receptionist);
    read_patient_receptionist_buffer(comm->patient_receptionist, data->buffers_size, ad);
    consume_end(sems->patient_receptionist);
}


void receptionist_process_admission(struct admission* ad, int receptionist_id, struct data_container* data, struct semaphores* sems) {
    ad->receiving_receptionist = receptionist_id;
    ad->status = 'R';
    get_receptionist_time(ad); //Temporização da admissão do lado do recepcionista

    semaphore_lock(sems->receptionist_stats_mutex);
    data->receptionist_stats[receptionist_id]++;
    semaphore_unlock(sems->receptionist_stats_mutex);

    semaphore_lock(sems->results_mutex);
    data->results[ad->id] = *ad;
    semaphore_unlock(sems->results_mutex);
}


void receptionist_send_admission(struct admission* ad, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    produce_begin(sems->receptionist_doctor);
    write_receptionist_doctor_buffer(comm->receptionist_doctor, data->buffers_size, ad);
    produce_end(sems->receptionist_doctor);
}