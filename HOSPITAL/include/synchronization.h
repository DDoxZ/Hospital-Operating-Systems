/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#ifndef SYNCHRONIZATION_H_GUARD
#define SYNCHRONIZATION_H_GUARD

#include <semaphore.h>
	
// Nomes usados na criacao dos semaforos
#define STR_SEM_MAIN_PATIENT_FULL       "SEM_MAIN_PATIENT_FULL"
#define STR_SEM_MAIN_PATIENT_EMPTY      "SEM_MAIN_PATIENT_EMPTY"
#define STR_SEM_MAIN_PATIENT_MUTEX      "SEM_MAIN_PATIENT_MUTEX"

#define STR_SEM_PATIENT_RECEPT_FULL     "SEM_PATIENT_RECEPT_FULL"
#define STR_SEM_PATIENT_RECEPT_EMPTY    "SEM_PATIENT_RECEPT_EMPTY"
#define STR_SEM_PATIENT_RECEPT_MUTEX    "SEM_PATIENT_RECEPT_MUTEX"

#define STR_SEM_RECEPT_DOCTOR_FULL 	    "SEM_RECEPT_DOCTOR_FULL"
#define STR_SEM_RECEPT_DOCTOR_EMPTY 	"SEM_RECEPT_DOCTOR_EMPTY"
#define STR_SEM_RECEPT_DOCTOR_MUTEX 	"SEM_RECEPT_DOCTOR_MUTEX"

#define STR_SEM_PATIENT_STATS_MUTEX		"SEM_PATIENT_STATS_MUTEX" 	// V3
#define STR_SEM_RECEPT_STATS_MUTEX		"SEM_RECEPT_STATS_MUTEX" 	// V3
#define STR_SEM_DOCTOR_STATS_MUTEX		"SEM_DOCTOR_STATS_MUTEX" 	// V3
#define STR_SEM_RESULTS_MUTEX			"SEM_RESULTS_MUTEX" 		// V3
#define STR_SEM_TERMINATE_MUTEX			"SEM_TERMINATE_MUTEX" 		// V3

//estrutura de 3 semáforos utilizada no modelo produtor/consumidor
struct prodcons {
	sem_t *full, *empty, *mutex;
};

//estrutura que agrega informação de todos os semáforos necessários pelo hOSpital
struct semaphores {
	struct prodcons *main_patient;	        // semáforos para acesso ao buffer entre a main e os pacientes
	struct prodcons *patient_receptionist;  // semáforos para acesso ao buffer entre os pacientes e os rececionistas
	struct prodcons *receptionist_doctor; 	// semáforos para acesso ao buffer entre os rececionistas e os médicos
	sem_t *patient_stats_mutex;				// V3
	sem_t *receptionist_stats_mutex;		// V3
	sem_t *doctor_stats_mutex;				// V3
	sem_t *results_mutex;					// V3
	sem_t *terminate_mutex;					// V3
};

/* Função que cria um novo semáforo com nome name e valor inicial igual a
* value. Pode concatenar o resultado da função getpid() a name, para tornar
* o nome único para o processo.
*/
sem_t * semaphore_create(char* name, int value);

/* Função que destroi o semáforo passado em argumento.
*/
void semaphore_destroy(char* name, sem_t* semaphore);

/* Função que inicia o processo de produzir, fazendo sem_wait nos semáforos
* corretos da estrutura passada em argumento.
*/
void produce_begin(struct prodcons* pc);

/* Função que termina o processo de produzir, fazendo sem_post nos semáforos
* corretos da estrutura passada em argumento.
*/
void produce_end(struct prodcons* pc);

/* Função que inicia o processo de consumir, fazendo sem_wait nos semáforos
* corretos da estrutura passada em argumento.
*/
void consume_begin(struct prodcons* pc);

/* Função que termina o processo de consumir, fazendo sem_post nos semáforos
* corretos da estrutura passada em argumento.
*/
void consume_end(struct prodcons* pc);

/* Função que faz wait a um semáforo.
*/
void semaphore_lock(sem_t* sem);

/* Função que faz post a um semáforo.
*/
void semaphore_unlock(sem_t* sem);


#endif
