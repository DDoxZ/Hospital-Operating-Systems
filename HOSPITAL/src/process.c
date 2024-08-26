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
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

#include "../include/process.h"
#include "../include/patient.h"
#include "../include/receptionist.h"
#include "../include/doctor.h"


int launch_patient(int patient_id, struct data_container* data, struct communication* comm, struct semaphores* sems){
    pid_t pid = fork();

    if(pid == -1){
        perror("[Process] Error launching patient process");
        exit(-1);
    }else if(pid == 0){
        exit(execute_patient(patient_id, data, comm, sems));
    }

    return pid;

}



int launch_receptionist(int receptionist_id, struct data_container* data, struct communication* comm, struct semaphores* sems){
    pid_t pid = fork();

    if(pid == -1){
        perror("[Process] Error launching receptionist process");
        exit(-1);
    }else if(pid == 0){
        exit(execute_receptionist(receptionist_id, data, comm, sems));
    }

    return pid;
}



int launch_doctor(int doctor_id, struct data_container* data, struct communication* comm, struct semaphores* sems){
    pid_t pid = fork();

    if(pid == -1){
        perror("[Process] Error launching doctor process");
        exit(-1);
    }else if(pid == 0){
        exit(execute_doctor(doctor_id, data, comm, sems));
    }

    return pid;
}


int wait_process(int process_id){
    int result;

    waitpid(process_id, &result, 0);
    if(WIFEXITED(result))
        return WEXITSTATUS(result);
    return -1;
}