/* ATENÇÃO: NÃO MODIFICAR ESTE FICHEIRO
 * Os ficheiros .h serão substituídos pelos fornecidos pelos professores durante a avaliação
 * Caso estes ficheiros sejam modificados, o trabalho deixará de compilar durante a avaliação
*/
#ifndef PROCESS_H_GUARD
#define PROCESS_H_GUARD

#include "memory.h"
#include "main.h"
#include "synchronization.h"

/* Função que inicia um novo processo Paciente através da função fork do SO. O novo
* processo irá executar a função execute_patient respetiva, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_patient(int patient_id, struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Função que inicia um novo processo Rececionista através da função fork do SO. O novo
* processo irá executar a função execute_receptionist, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_receptionist(int receptionist_id, struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Função que inicia um novo processo Médico através da função fork do SO. O novo
* processo irá executar a função execute_doctor, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_doctor(int doctor_id, struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Função que espera que um processo termine através da função waitpid. 
* Devolve o retorno do processo, se este tiver terminado normalmente.
*/
int wait_process(int process_id);

#endif
