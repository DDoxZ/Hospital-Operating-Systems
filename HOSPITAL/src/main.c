/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include "../include/main.h"
#include "../include/process.h"
#include "../include/configuration.h"
#include "../include/memory.h"
#include "../include/hosptime.h"
#include "../include/log.h"
#include "../include/synchronization.h"
#include "../include/hospsignal.h" 
#include "../include/stats.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int* ad_counter;

int main(int argc, char *argv[]) {
    //init data structures
    struct data_container* data = allocate_dynamic_memory(sizeof(struct data_container));
    struct communication* comm = allocate_dynamic_memory(sizeof(struct communication));
    comm->main_patient = allocate_dynamic_memory(sizeof(struct circular_buffer));
    comm->patient_receptionist = allocate_dynamic_memory(sizeof(struct rnd_access_buffer));
    comm->receptionist_doctor = allocate_dynamic_memory(sizeof(struct circular_buffer));
    struct semaphores* sems = allocate_dynamic_memory(sizeof(struct semaphores));
    sems->main_patient = allocate_dynamic_memory(sizeof(struct prodcons));
    sems->patient_receptionist = allocate_dynamic_memory(sizeof(struct prodcons));
    sems->receptionist_doctor = allocate_dynamic_memory(sizeof(struct prodcons));
    
    ad_counter = allocate_dynamic_memory(sizeof(int));
    *ad_counter = 0;

    main_args(argc, argv, data);
    allocate_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, comm);
    create_semaphores(data, sems);

    launch_processes(data, comm, sems);
    user_interaction(data, comm, sems);
}

void main_args(int argc, char* argv[], struct data_container* data) {
    // usage string
    char usage[] = "[Main] Uso: ./HOSPITAL config_file \n[Main] Exemplo ./bin/HOSPITAL config.txt\n";

    if(argc != 2)
    {
        printf("%s", usage);
        exit(-1);
    }

    if (read_config_file(argv[1], data) == -1) {
        //printf("[Main] Erro ao ler o ficheiro de configuração!\n");
        exit(-1);
    }

    printf("[Main] Configuração lida com sucesso!\n");
}

void allocate_dynamic_memory_buffers(struct data_container* data) {
    data->patient_pids = allocate_dynamic_memory(sizeof(int) * data->n_patients);
    data->receptionist_pids = allocate_dynamic_memory(sizeof(int) * data->n_receptionists);
    data->doctor_pids = allocate_dynamic_memory(sizeof(int) * data->n_doctors);
}

void create_shared_memory_buffers(struct data_container* data, struct communication* comm) {
    long size = sizeof(struct admission) * data->buffers_size * 1000;

    // circular buffer
    comm->main_patient->ptrs = create_shared_memory(STR_SHM_MAIN_PATIENT_PTR, sizeof(struct pointers));
    comm->main_patient->buffer = create_shared_memory(STR_SHM_MAIN_PATIENT_BUFFER, size);
    comm->main_patient->ptrs->in = 0;
    comm->main_patient->ptrs->out = 0;

    // random access buffer
    comm->patient_receptionist->ptrs = create_shared_memory(STR_SHM_PATIENT_RECEPT_PTR, sizeof(int*) * data->buffers_size);
    comm->patient_receptionist->buffer = create_shared_memory(STR_SHM_PATIENT_RECEPT_BUFFER, size);

    // circular buffer
    comm->receptionist_doctor->ptrs = create_shared_memory(STR_SHM_RECEPT_DOCTOR_PTR, sizeof(struct pointers));
    comm->receptionist_doctor->buffer = create_shared_memory(STR_SHM_RECEPT_DOCTOR_BUFFER, size);
    comm->receptionist_doctor->ptrs->in = 0;
    comm->receptionist_doctor->ptrs->out = 0;

    data->patient_stats = create_shared_memory(STR_SHM_PATIENT_STATS, sizeof(int) * data->n_patients);
    data->receptionist_stats = create_shared_memory(STR_SHM_RECEPT_STATS, sizeof(int) * data->n_receptionists);
    data->doctor_stats = create_shared_memory(STR_SHM_DOCTOR_STATS, sizeof(int) * data->n_doctors);

    data->results = create_shared_memory(STR_SHM_RESULTS, sizeof(struct admission) * MAX_RESULTS);

    data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
}

void launch_processes(struct data_container* data, struct communication* comm, struct semaphores* sems) {
    // launch patient processes
    for (int i = 0; i < data->n_patients; i++) {
        int pid = launch_patient(i, data, comm, sems);
        data->patient_pids[i] = pid;
        data->patient_stats[i] = 0;
    }

    // launch receptionist processes
    for (int i = 0; i < data->n_receptionists; i++) {
        int pid = launch_receptionist(i, data, comm, sems);
        data->receptionist_pids[i] = pid;
        data->receptionist_stats[i] = 0;
    }

    // launch doctor processes
    for (int i = 0; i < data->n_doctors; i++) {
        int pid = launch_doctor(i, data, comm, sems);
        data->doctor_pids[i] = pid;
        data->doctor_stats[i] = 0;
    }
}

void user_interaction(struct data_container* data, struct communication* comm, struct semaphores* sems) {
    char commands_explanation[] = "[Main] Ações disponíveis:\n"
        "[Main]  ad paciente medico - criar uma nova admissão.\n"
        "[Main]  info id - consultar o estado de uma admissão.\n"
        "[Main]  status - imprime informação sobre o estado do hospital.\n"
        "[Main]  help - imprime informação sobre as ações disponíveis.\n"
        "[Main]  end - termina a execução do hOSpital.\n";

    printf("%s", commands_explanation);

    *ad_counter = 0;
    launch_signals(data->alarm_time, ad_counter, data, comm, sems);
    delete_log(data->log_filename);

    while(1) {
        char command[100];
        printf("\n[Main] Introduzir ação: ");

        // read command
        scanf("%s", command);
        
        if(strcmp(command, "ad") == 0) {
            create_request(ad_counter, data, comm, sems);
            usleep(80000);

        } else if(strcmp(command, "info") == 0) {
            read_info(data, sems);

        } else if(strcmp(command, "status") == 0) {
            print_status(data, sems);

        } else if(strcmp(command, "end") == 0) {
            end_execution(data, comm, sems);
            break;
            
        } else if(strcmp(command, "help") == 0) {
            printf("%s", commands_explanation);
            write_log(data->log_filename, "help", NULL);
        } else {
            printf("\n[Main] Ação não reconhecida, insira 'help' para assistência.\n");
        }
    }

    deallocate_dynamic_memory(ad_counter);
}

void create_request(int* ad_counter, struct data_container* data, struct communication* comm, struct semaphores* sems) {
    // read patient id from scanf
    int patient_id;
    int doctor_id;

    scanf("%d %d", &patient_id, &doctor_id);
    
    // if (*ad_counter >= data->max_ads) {
    //     printf("[Main] O número máximo de admissões foi alcançado!\n");
    // }

    produce_begin(sems->main_patient);
    semaphore_lock(sems->results_mutex);

    // create admission
    struct admission *new_ad = allocate_dynamic_memory(sizeof(struct admission));
    new_ad->id = *ad_counter;
    new_ad->requesting_patient = patient_id;
    new_ad->requested_doctor = doctor_id;
    new_ad->status = 'M';
    get_create_time(new_ad); //Temporização

    // print trying to create admission
    printf("[Main] A criar admissão %d para o paciente %d destinada ao médico %d!\n", new_ad->id, new_ad->requesting_patient, new_ad->requested_doctor);

    write_main_patient_buffer(comm->main_patient, data->buffers_size, new_ad);

    data->results[new_ad->id] = *new_ad;

    // print admission created
    printf("[Main] A admissão %d foi criada!\n", new_ad->id);

    // increment admission counter
    int arguments[2] = {patient_id, doctor_id};
    write_log(data->log_filename, "ad", arguments);
    *ad_counter += 1;

    semaphore_unlock(sems->results_mutex);
    produce_end(sems->main_patient);
}

void read_info(struct data_container* data, struct semaphores* sems) {
    int id;
    scanf("%d", &id);

    // if (id+1 > data->max_ads) {
    //     printf("[Main] Admissão %d possui um id maior ou igual ao máximo de atendimentos previstos para a execução (%d)!\n", id, data->max_ads);
    //     return;
    // }

    semaphore_lock(sems->results_mutex);
    struct admission* ad = &data->results[id];
    semaphore_unlock(sems->results_mutex);

    if (ad->status == 'M') {
        printf("[Main] A admissão %d com estado %c requisitada pelo paciente %d ao médico %d, foi enviada ao paciente!\n", ad->id, ad->status, ad->requesting_patient, ad->requested_doctor);

    } else if (ad->status == 'P') {
        printf("[Main] A admissão %d com estado %c requisitada pelo paciente %d ao médico %d, foi recebida pelo paciente %d!\n", ad->id, ad->status, ad->requesting_patient, ad->requested_doctor, ad->receiving_patient);

    } else if (ad->status == 'R') {
        printf("[Main] A admissão %d com estado %c requisitada pelo paciente %d ao médico %d, foi recebida pelo paciente %d e admitida pelo rececionista %d!\n", ad->id, ad->status, ad->requesting_patient, ad->requested_doctor, ad->receiving_patient, ad->receiving_receptionist);

    } else if (ad->status == 'A' || ad->status == 'N') {
        printf("[Main] A admissão %d com estado %c requisitada pelo paciente %d ao médico %d, foi recebida pelo paciente %d, admitida pelo rececionista %d, e concluída pelo médico %d!\n", ad->id, ad->status, ad->requesting_patient, ad->requested_doctor, ad->receiving_patient, ad->receiving_receptionist, ad->receiving_doctor);

    } else {
        printf("[Main] A admissão ainda não existe!");
    }
    int argument[1] = {id};
    write_log(data->log_filename, "info", argument);
}

void print_status(struct data_container* data, struct semaphores* sems) {
    printf("[Main] max_ads: %d\n", data->max_ads);
    printf("[Main] buffers_size: %d\n", data->buffers_size);
    printf("[Main] n_patients: %d\n", data->n_patients);
    printf("[Main] n_receptionists: %d\n", data->n_receptionists);
    printf("[Main] n_doctors: %d\n", data->n_doctors);

    printf("[Main] patient_pids: [");
    for (int i = 0; i < data->n_patients; i++) {
        printf("%d", data->patient_pids[i]);
        if (i != data->n_patients - 1) printf(", ");
    }
    printf("]\n");

    printf("[Main] receptionist_pids: [");
    for (int i = 0; i < data->n_receptionists; i++) {
        printf("%d", data->receptionist_pids[i]);
        if (i != data->n_receptionists - 1) printf(", ");
    }
    printf("]\n");

    printf("[Main] doctor_pids: [");
    for (int i = 0; i < data->n_doctors; i++) {
        printf("%d", data->doctor_pids[i]);
        if (i != data->n_doctors - 1) printf(", ");
    }
    printf("]\n");

    semaphore_lock(sems->patient_stats_mutex);
    printf("[Main] patient_stats: [");
    for (int i = 0; i < data->n_patients; i++) {
        printf("%d", data->patient_stats[i]);
        if (i != data->n_patients - 1) printf(", ");
    }
    printf("]\n");
    semaphore_unlock(sems->patient_stats_mutex);
    
    semaphore_lock(sems->receptionist_stats_mutex);
    printf("[Main] receptionist_stats: [");
    for (int i = 0; i < data->n_receptionists; i++) {
        printf("%d", data->receptionist_stats[i]);
        if (i != data->n_receptionists - 1) printf(", ");
    }
    printf("]\n");
    semaphore_unlock(sems->receptionist_stats_mutex);

    semaphore_lock(sems->doctor_stats_mutex);
    printf("[Main] doctor_stats: [");
    for (int i = 0; i < data->n_doctors; i++) {
        printf("%d", data->doctor_stats[i]);
        if (i != data->n_doctors - 1) printf(", ");
    }
    printf("]\n");
    semaphore_unlock(sems->doctor_stats_mutex);

    semaphore_lock(sems->results_mutex);
    printf("[Main] results: [");
    for (int i = 0; i < data->max_ads; i++) {
        printf("%d", data->results[i].id);
        if (i != data->max_ads - 1) printf(", ");
    }
    printf("]\n");
    semaphore_unlock(sems->results_mutex);

    printf("[Main] terminate: %d\n", *data->terminate);

    write_log(data->log_filename, "status", NULL);
}

void end_execution(struct data_container* data, struct communication* comm, struct semaphores* sems) {
    semaphore_lock(sems->terminate_mutex);
    *data->terminate = 1;
    semaphore_unlock(sems->terminate_mutex);

    wakeup_processes(data, sems);
    wait_processes(data);
    //write_statistics(data);
    write_log(data->log_filename, "end", NULL);
    process_statistics(data, ad_counter);
    destroy_memory_buffers(data, comm);
    destroy_semaphores(sems);
    printf("\n[Main] hOSpital encerrado!\n");
    
    //release memory before terminating
    deallocate_dynamic_memory(data);
    deallocate_dynamic_memory(comm->main_patient);
    deallocate_dynamic_memory(comm->patient_receptionist);
    deallocate_dynamic_memory(comm->receptionist_doctor);
    deallocate_dynamic_memory(comm);
    deallocate_dynamic_memory(sems->main_patient);
    deallocate_dynamic_memory(sems->patient_receptionist);
    deallocate_dynamic_memory(sems->receptionist_doctor);
    deallocate_dynamic_memory(sems);
    deallocate_dynamic_memory(ad_counter);

    exit(0);
}

void wait_processes(struct data_container* data) {
    for (int i = 0; i < data->n_patients; i++) {
        wait_process(data->patient_pids[i]);
    }

    for (int i = 0; i < data->n_receptionists; i++) {
        wait_process(data->receptionist_pids[i]);
    }

    for (int i = 0; i < data->n_doctors; i++) {
        wait_process(data->doctor_pids[i]);
    }
}

/* Função que imprime as estatisticas finais do hOSpital, nomeadamente quantas admissões foram solicitadas por cada paciente, realizadas por cada rececionista e atendidas por cada médico. */
void write_statistics(struct data_container* data) {    
    printf("[Main] A encerrar a execução do hOSpital! As estatísticas da execução são:\n");

    for (int i = 0; i < data->n_patients; i++) {
        printf("[Main] O paciente %d requeriu %d admissões!\n", i, data->patient_stats[i]);
    }

    for (int i = 0; i < data->n_receptionists; i++) {
        printf("[Main] O rececionista %d realizou %d admissões!\n", i, data->receptionist_stats[i]);
    }

    for (int i = 0; i < data->n_doctors; i++) {
        printf("[Main] O médico %d atendeu %d admissões!\n", i, data->doctor_stats[i]);
    }
}

void create_semaphores(struct data_container* data, struct semaphores* sems) {
    sems->main_patient = allocate_dynamic_memory(sizeof(struct prodcons));
    sems->main_patient->full = semaphore_create(STR_SEM_MAIN_PATIENT_FULL, 0);
    sems->main_patient->empty = semaphore_create(STR_SEM_MAIN_PATIENT_EMPTY, data->buffers_size);
    sems->main_patient->mutex = semaphore_create(STR_SEM_MAIN_PATIENT_MUTEX, 1);

    sems->patient_receptionist = allocate_dynamic_memory(sizeof(struct prodcons));
    sems->patient_receptionist->full = semaphore_create(STR_SEM_PATIENT_RECEPT_FULL, 0);
    sems->patient_receptionist->empty = semaphore_create(STR_SEM_PATIENT_RECEPT_EMPTY, data->buffers_size);
    sems->patient_receptionist->mutex = semaphore_create(STR_SEM_PATIENT_RECEPT_MUTEX, 1);

    sems->receptionist_doctor = allocate_dynamic_memory(sizeof(struct prodcons));
    sems->receptionist_doctor->full = semaphore_create(STR_SEM_RECEPT_DOCTOR_FULL, 0);
    sems->receptionist_doctor->empty = semaphore_create(STR_SEM_RECEPT_DOCTOR_EMPTY, data->buffers_size);
    sems->receptionist_doctor->mutex = semaphore_create(STR_SEM_RECEPT_DOCTOR_MUTEX, 1);
    
    sems->patient_stats_mutex = semaphore_create(STR_SEM_PATIENT_STATS_MUTEX, 1);
    sems->receptionist_stats_mutex = semaphore_create(STR_SEM_RECEPT_STATS_MUTEX, 1);
    sems->doctor_stats_mutex = semaphore_create(STR_SEM_DOCTOR_STATS_MUTEX, 1);

    sems->results_mutex = semaphore_create(STR_SEM_RESULTS_MUTEX, 1);
    sems->terminate_mutex = semaphore_create(STR_SEM_TERMINATE_MUTEX, 1);
}

void destroy_semaphores(struct semaphores* sems) {
    semaphore_destroy(STR_SEM_MAIN_PATIENT_FULL, sems->main_patient->full);
    semaphore_destroy(STR_SEM_MAIN_PATIENT_EMPTY, sems->main_patient->empty);
    semaphore_destroy(STR_SEM_MAIN_PATIENT_MUTEX, sems->main_patient->mutex);

    semaphore_destroy(STR_SEM_PATIENT_RECEPT_FULL, sems->patient_receptionist->full);
    semaphore_destroy(STR_SEM_PATIENT_RECEPT_EMPTY, sems->patient_receptionist->empty);
    semaphore_destroy(STR_SEM_PATIENT_RECEPT_MUTEX, sems->patient_receptionist->mutex);

    semaphore_destroy(STR_SEM_RECEPT_DOCTOR_FULL, sems->receptionist_doctor->full);
    semaphore_destroy(STR_SEM_RECEPT_DOCTOR_EMPTY, sems->receptionist_doctor->empty);
    semaphore_destroy(STR_SEM_RECEPT_DOCTOR_MUTEX, sems->receptionist_doctor->mutex);

    semaphore_destroy(STR_SEM_PATIENT_STATS_MUTEX, sems->patient_stats_mutex);
    semaphore_destroy(STR_SEM_RECEPT_STATS_MUTEX, sems->receptionist_stats_mutex);
    semaphore_destroy(STR_SEM_DOCTOR_STATS_MUTEX, sems->doctor_stats_mutex);

    semaphore_destroy(STR_SEM_RESULTS_MUTEX, sems->results_mutex);
    semaphore_destroy(STR_SEM_TERMINATE_MUTEX, sems->terminate_mutex);
}

void wakeup_processes(struct data_container* data, struct semaphores* sems) {
    for (int i = 0; i < data->n_patients; i++) {
        produce_end(sems->main_patient);
    }

    for (int i = 0; i < (data->n_receptionists + data-> n_patients); i++) {
        produce_end(sems->patient_receptionist);
    }

    for (int i = 0; i < (data->n_doctors + data->n_receptionists); i++) {
        produce_end(sems->receptionist_doctor);
    }
}

void destroy_memory_buffers(struct data_container* data, struct communication* comm) {
    destroy_shared_memory(STR_SHM_MAIN_PATIENT_PTR, comm->main_patient->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_MAIN_PATIENT_BUFFER, comm->main_patient->buffer, sizeof(struct admission) * data->buffers_size);
    
    destroy_shared_memory(STR_SHM_PATIENT_RECEPT_PTR, comm->patient_receptionist->ptrs, sizeof(int*) * data->buffers_size);
    destroy_shared_memory(STR_SHM_PATIENT_RECEPT_BUFFER, comm->patient_receptionist->buffer, sizeof(struct admission) * data->buffers_size);

    destroy_shared_memory(STR_SHM_RECEPT_DOCTOR_PTR, comm->receptionist_doctor->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_RECEPT_DOCTOR_BUFFER, comm->receptionist_doctor->buffer, sizeof(struct admission) * data->buffers_size);

    destroy_shared_memory(STR_SHM_PATIENT_STATS, data->patient_stats, sizeof(int) * data->n_patients);
    destroy_shared_memory(STR_SHM_RECEPT_STATS, data->receptionist_stats, sizeof(int) * data->n_receptionists);
    destroy_shared_memory(STR_SHM_DOCTOR_STATS, data->doctor_stats, sizeof(int) * data->n_doctors);

    destroy_shared_memory(STR_SHM_RESULTS, data->results, sizeof(struct admission) * MAX_RESULTS);
    
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));

    deallocate_dynamic_memory(data->patient_pids);
    deallocate_dynamic_memory(data->receptionist_pids);
    deallocate_dynamic_memory(data->doctor_pids);
}
