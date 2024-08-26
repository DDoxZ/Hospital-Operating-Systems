/* ATENÇÃO: NÃO MODIFICAR ESTE FICHEIRO
 * Os ficheiros .h serão substituídos pelos fornecidos pelos professores durante a avaliação
 * Caso estes ficheiros sejam modificados, o trabalho deixará de compilar durante a avaliação
*/
#ifndef RECEPTIONIST_H_GUARD
#define RECEPTIONIST_H_GUARD

#include "memory.h"
#include "main.h"
#include "synchronization.h"

/* Função principal de um Rececionista. Deve executar um ciclo infinito onde em 
* cada iteração lê uma admissão dos pacientes e se a mesma tiver id 
* diferente de -1 e se data->terminate ainda for igual a 0, processa-a e
* a encaminha para os médicos. Admissões com id igual a -1 são 
* ignoradas (ad inválida) e se data->terminate for igual a 1 é porque foi 
* dada ordem de terminação do programa, portanto deve-se fazer return do
* número de admissões realizadas. Para efetuar estes passos, pode usar os
* outros métodos auxiliares definidos em receptionist.h.
*/
int execute_receptionist(int receptionist_id, struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Função que lê uma admissão do buffer de memória partilhada entre os pacientes e os rececionistas.
* Antes de tentar ler a admissão, deve verificar se data->terminate tem valor 1.
* Em caso afirmativo, retorna imediatamente da função.
*/
void receptionist_receive_admission(struct admission* ad, struct data_container* data, struct communication* comm, struct semaphores* sems);

/* Função que realiza uma admissão, alterando o seu campo receiving_receptionist para o id
* passado como argumento, alterando o estado da mesma para 'R' (receptionist), e 
* incrementando o contador de admissões realizadas por este rececionista no data_container. 
* Atualiza também a admissão na estrutura data.
*/
void receptionist_process_admission(struct admission* ad, int receptionist_id, struct data_container* data, struct semaphores* sems);

/* Função que escreve uma admissão no buffer de memória partilhada entre
* os rececionistas e os médicos.
*/
void receptionist_send_admission(struct admission* ad, struct data_container* data, struct communication* comm, struct semaphores* sems);

#endif
