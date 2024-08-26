/* ATENÇÃO: NÃO MODIFICAR ESTE FICHEIRO
 * Os ficheiros .h serão substituídos pelos fornecidos pelos professores durante a avaliação
 * Caso estes ficheiros sejam modificados, o trabalho deixará de compilar durante a avaliação
*/
#ifndef MAIN_H_GUARD
#define MAIN_H_GUARD

#include "memory.h"
#include "synchronization.h"

//Estrutura que agrega a informação necessária pela main do hOSpital.
struct data_container {
	int max_ads;					//número máximo de admissões durante a execução
	int buffers_size;				//tamanho máximo dos buffers de mem. partilhada
	char log_filename[256];			//v4 nome do ficheiro log
	char statistics_filename[256];	//v4 nome do ficheiro de estatisticas
	int alarm_time;					//v4 numero de intervalo do alarme 
	
	int n_patients;				//número de pacientes
	int n_receptionists;		//número de rececionistas
	int n_doctors;				//número de médicos
	
	int *patient_pids;			//process ids dos pacientes
	int *receptionist_pids;		//process ids de rececionistas
	int *doctor_pids;			//process ids de médicos
	
	int* patient_stats;			//array com nº de admissões solicitadas por cada paciente
	int* receptionist_stats;	//array com nº de admissões encaminhadas por cada rececionista
	int* doctor_stats;			//array com nº de admissões atendidas por cada médico
	
	struct admission* results;	//array com histórico de ops executadas
	
	int* terminate; //flag booleana, valor 1 indica que o hOSpital deve terminar a sua execução
};

/* Função que lê o argumento da aplicação, nomeadamente o ficheiro de configuração
* com o número máximo de admissões, o tamanho dos buffers de memória partilhada
* usados para comunicação, e o número de pacientes, de rececionistas e de
* médicos, entre outros. Guarda esta informação nos campos apropriados da
* estrutura data_container.
*/
void main_args(int argc, char* argv[], struct data_container* data);

/* Função que reserva a memória dinâmica necessária para a execução
* do hOSpital, nomeadamente para os arrays *_pids da estrutura 
* data_container. Para tal, pode ser usada a função allocate_dynamic_memory.
*/
void allocate_dynamic_memory_buffers(struct data_container* data);

/* Função que reserva a memória partilhada necessária para a execução do
* hOSpital. É necessário reservar memória partilhada para todos os buffers da
* estrutura communication, incluindo os buffers em si e respetivos
* pointers, assim como para os arrays data->results, *_stats e 
* a variável data->terminate. Para tal, pode ser usada a função create_shared_memory.
*/
void create_shared_memory_buffers(struct data_container* data, struct communication* comm);

/* Função que inicia os processos dos pacientes, rececionistas e
* médicos. Para tal, pode usar as funções launch_*,
* guardando os pids resultantes nos arrays respetivos
* da estrutura data_container.
*/
void launch_processes(struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Função que faz a interação do utilizador, podendo receber 5 comandos:
* ad paciente médico - cria uma nova admissão, através da função create_request
* info - verifica o estado de uma admissão através da função read_info
* status - apresenta o estado atual das variáveis contidas no data_container
* help - imprime informação sobre os comandos disponiveis
* end - termina o execução do hOSpital através da função stop_execution
*/
void user_interaction(struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Cria uma nova admissão identificada pelo valor atual de ad_counter e com os 
* dados introduzidos pelo utilizador na linha de comandos, escrevendo a mesma 
* no buffer de memória partilhada entre a main e os pacientes. Imprime o id da 
* admissão e incrementa o contador de admissões ad_counter.
*/
void create_request(int* ad_counter, struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Função que lê um id de admissão do utilizador e verifica se a mesma é valida.
* Em caso afirmativo imprime a informação da mesma, nomeadamente o seu estado, o 
* id do paciente que fez o pedido, o id do médico requisitado, e os ids do paciente,
* rececionista, e médico que a receberam e processaram.
*/
void read_info(struct data_container* data, struct semaphores* sems);

/* Função que imprime o estado do data_container, nomeadamente todos os seus campos.
* No caso dos arrays, deve-se imprimir no formato [0, 1, 2, ..., N], onde N é o último elemento do array.
*/
void print_status(struct data_container* data, struct semaphores* sems);

/* Função que termina a execução do programa hOSpital. Deve começar por 
* afetar a flag data->terminate com o valor 1. De seguida, e por esta
* ordem, deve esperar que os processos filho terminem, deve escrever as
* estatisticas finais do programa, e por fim libertar
* as zonas de memória partilhada e dinâmica previamente 
* reservadas. Para tal, pode usar as outras funções auxiliares do main.h.
*/
void end_execution(struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Função que espera que todos os processos previamente iniciados terminem,
* incluindo pacientes, rececionistas e médicos. Para tal, pode usar a função 
* wait_process do process.h.
*/
void wait_processes(struct data_container* data);

/* Função que imprime as estatisticas finais do hOSpital, nomeadamente quantas
* admissões foram solicitadas por cada paciente, realizadas por cada rececionista
* e atendidas por cada médico.
*/
void write_statistics(struct data_container* data);

/* Função que liberta todos os buffers de memória dinâmica e partilhada previamente
* reservados na estrutura data.
*/
void destroy_memory_buffers(struct data_container* data, struct communication* comm);

/* Função que inicializa os semáforos da estrutura semaphores. Semáforos
* *_full devem ser inicializados com valor 0, semáforos *_empty com valor
* igual ao tamanho dos buffers de memória partilhada, e os *_mutex com
* valor igual a 1. Para tal pode ser usada a função semaphore_create.*/
void create_semaphores(struct data_container* data, struct semaphores* sems);

/* Função que acorda todos os processos adormecidos em semáforos, para que
* estes percebam que foi dada ordem de terminação do programa. Para tal,
* pode ser usada a função produce_end sobre todos os conjuntos de semáforos
* onde possam estar processos adormecidos e um número de vezes igual ao
* máximo de processos que possam lá estar.*/
void wakeup_processes(struct data_container* data, struct semaphores* sems);

/* Função que liberta todos os semáforos da estrutura semaphores. */
void destroy_semaphores(struct semaphores* sems);

#endif
