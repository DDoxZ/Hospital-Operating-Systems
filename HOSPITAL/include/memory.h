/* ATENÇÃO: NÃO MODIFICAR ESTE FICHEIRO
 * Os ficheiros .h serão substituídos pelos fornecidos pelos professores durante a avaliação
 * Caso estes ficheiros sejam modificados, o trabalho deixará de compilar durante a avaliação
*/
#ifndef MEMORY_H_GUARD
#define MEMORY_H_GUARD

#include <time.h>	// V2

#define STR_SHM_MAIN_PATIENT_PTR 				"SHM_MAIN_PATIENT_PTR"
#define STR_SHM_MAIN_PATIENT_BUFFER 			"SHM_MAIN_PATIENT_BUFFER"
#define STR_SHM_PATIENT_RECEPT_PTR 				"SHM_PATIENT_RECEPT_PTR"
#define STR_SHM_PATIENT_RECEPT_BUFFER 			"SHM_PATIENT_RECEPT_BUFFER"
#define STR_SHM_RECEPT_DOCTOR_PTR 				"SHM_RECEPT_DOCTOR_PTR"
#define STR_SHM_RECEPT_DOCTOR_BUFFER 			"SHM_RECEPT_DOCTOR_BUFFER"
#define STR_SHM_PATIENT_STATS					"SHM_PATIENT_STATS"
#define STR_SHM_RECEPT_STATS					"SHM_RECEPT_STATS"
#define STR_SHM_DOCTOR_STATS					"SHM_DOCTOR_STATS"
#define STR_SHM_RESULTS							"SHM_RESULTS"
#define STR_SHM_TERMINATE						"SHM_TERMINATE"

#define MAX_RESULTS 100		//constante usada para limitar tamanho de buffer de resultados

struct pointers { 																					
	int in;  																						
	int out; 																						
};																								

//estrutura que representa um buffer circular
struct circular_buffer { 	
	struct pointers *ptrs;
	struct admission* buffer;
};

//estrutura que representa um buffer de acesso aleatório
struct rnd_access_buffer {
	int* ptrs;
	struct admission* buffer;
};

//Estrutura que representa uma admissão
struct admission {
	int id; 					//id da admissão
	int requesting_patient;		//id do paciente que pediu a admissão
	int requested_doctor;		//id do médico pretendido
		
	char status;				//estado da admissão
	int receiving_patient;		//id do paciente que recebeu a admissão
	int receiving_receptionist;	//id do rececionista que realizou a admissão
	int receiving_doctor;		//id do médico que realizou a consulta

	struct timespec create_time; //quando a admissão foi criada
	struct timespec patient_time; //quando o paciente recebeu a admissão
	struct timespec receptionist_time; //quando o rececionista recebeu a admissão
	struct timespec doctor_time; //quando o médico recebeu a admissão
};

//estrutura que agrega os shared memory buffers necessários para comunicação entre processos
struct communication {
	struct circular_buffer* main_patient; 			//buffer para a main enviar os pedidos de admissão aos pacientes
	struct rnd_access_buffer* patient_receptionist;	//buffer para os pacientes encaminharem as admissões aos rececionistas
	struct circular_buffer* receptionist_doctor; 	//buffer para os rececionistas encaminharem as admissões aos médicos
};

/* Função que reserva uma zona de memória partilhada com tamanho indicado
* por size e nome name, preenche essa zona de memória com o valor 0, e 
* retorna um apontador para a mesma. Pode concatenar o resultado da função
* getpid() a name, para tornar o nome único para o processo.
*/
void* create_shared_memory(char* name, int size);

/* Função que reserva uma zona de memória dinâmica com tamanho indicado
* por size, preenche essa zona de memória com o valor 0, e retorna um 
* apontador para a mesma.
*/
void* allocate_dynamic_memory(int size);

/* Função que liberta uma zona de memória partilhada previamente reservada.
*/
void destroy_shared_memory(char* name, void* ptr, int size);

/* Função que liberta uma zona de memória dinâmica previamente reservada.
*/
void deallocate_dynamic_memory(void* ptr);

/* Função que escreve uma admissão no buffer de memória partilhada entre a Main
* e os pacientes. A admissão deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_main_patient_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad);

/* Função que escreve uma admissão no buffer de memória partilhada entre os pacientes
* e os rececionistas. A admissão deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad);

/* Função que escreve uma admissão no buffer de memória partilhada entre os rececionistas
* e os médicos. A admissão deve ser escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_receptionist_doctor_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad);

/* Função que lê uma admissão do buffer de memória partilhada entre a Main
* e os pacientes, se houver alguma disponível para ler que seja direcionada ao paciente especificado.
* A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo.
* Se não houver nenhuma admissão disponível, afeta ad->id com o valor -1.
*/
void read_main_patient_buffer(struct circular_buffer* buffer, int patient_id, int buffer_size, struct admission* ad);

/* Função que lê uma admissão do buffer de memória partilhada entre os pacientes e rececionistas,
* se houver alguma disponível para ler (qualquer rececionista pode ler qualquer admissão).
* A leitura deve ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo.
* Se não houver nenhuma admissão disponível, afeta ad->id com o valor -1.
*/
void read_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad);

/* Função que lê uma admissão do buffer de memória partilhada entre os rececionistas e os médicos,
* se houver alguma disponível para ler dirigida ao médico especificado. A leitura deve
* ser feita tendo em conta o tipo de buffer e as regras de leitura em buffers desse tipo. Se não houver
* nenhuma admissão disponível, afeta ad->id com o valor -1.
*/
void read_receptionist_doctor_buffer(struct circular_buffer* buffer, int doctor_id, int buffer_size, struct admission* ad);

#endif
