/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include "../include/hospsignal.h"

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

static struct data_container* data_pointer;
static int* admissions_counter;
static struct communication* comm_pointer;
static struct semaphores* sems_pointer;

static void write_admissions() {
  for (int ad = 0; ad <= *admissions_counter; ad++) {
    // case 'M' e inicio de todos os outros
    printf("\n");

    printf("ad:%d ", data_pointer->results[ad].id);
    printf("status:%c ", data_pointer->results[ad].status);
    printf("start_time:%ld ", data_pointer->results[ad].create_time.tv_sec);

    switch (data_pointer->results[ad].status) {
      case 'P':
        printf("patient:%d ", data_pointer->results[ad].receiving_patient);
        printf("patient_time:%ld ", data_pointer->results[ad].patient_time.tv_sec);
        break;
      case 'R':
        printf("patient:%d ", data_pointer->results[ad].receiving_patient);
        printf("patient_time:%ld ", data_pointer->results[ad].patient_time.tv_sec);
        printf("receptionist:%d ", data_pointer->results[ad].receiving_receptionist);
        printf("receptionist_time:%ld ", data_pointer->results[ad].receptionist_time.tv_sec);
        break;
      case 'N': // para cair para A pq tem o mesmo resultado
      case 'A':
        printf("patient:%d ", data_pointer->results[ad].receiving_patient);
        printf("patient_time:%ld ", data_pointer->results[ad].patient_time.tv_sec);
        printf("receptionist:%d ", data_pointer->results[ad].receiving_receptionist);
        printf("receptionist_time:%ld ", data_pointer->results[ad].receptionist_time.tv_sec);
        printf("doctor:%d ", data_pointer->results[ad].receiving_doctor);
        printf("doctor_time:%ld ", data_pointer->results[ad].doctor_time.tv_sec);
        break;
    }
  }
}

void end_execution_wrapper() {
    end_execution(data_pointer, comm_pointer, sems_pointer);
}

void launch_signals(int alarm_time, int* ad_counter, struct data_container* data, struct communication* comm, struct semaphores* sems) {
  struct sigaction sa_admissions, sa_close_hospital;
  struct itimerval val;

  data_pointer = data;
  admissions_counter = ad_counter; 
  comm_pointer = comm;
  sems_pointer = sems;

  sa_admissions.sa_handler = write_admissions;
  sa_admissions.sa_flags = SA_RESTART;
  sigemptyset(&sa_admissions.sa_mask);

  sa_close_hospital.sa_handler = end_execution_wrapper;
  sa_close_hospital.sa_flags = SA_RESTART;
  sigemptyset(&sa_close_hospital.sa_mask);

  val.it_value.tv_sec = alarm_time;          // tempo ate primeira expiracao em segundos
  val.it_value.tv_usec = 0;                  // tempo ate primeira expiracao em microsegundos
  val.it_interval.tv_sec = alarm_time;       // tempo entre expiracoes em segundos
  val.it_interval.tv_usec = 0;               // tempo entre expiracoes em microsegundos
  
  if (setitimer(ITIMER_REAL, &val, 0) == -1) {
    perror("setitimer:");
    exit(-1);
  }

  if (sigaction(SIGALRM, &sa_admissions, NULL) == -1) {
    perror("sa_admissions:");
    exit(-1);
  }

  if (sigaction(SIGINT, &sa_close_hospital, NULL) == -1) {
    perror("sa_close_hospital:");
    exit(-1);
  }

}
