/**
 * Projeto SO - Fase 2
 * Grupo 10
 * Daniela Camarinha 58199
 * Diogo Lopes 60447
 * Pedro Silva 59886
 * 
*/

#include "../include/memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void* create_shared_memory(char* name, int size) {
    int fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("Erro ao criar memória partilhada.\n");
        exit(1);
    }

    if (ftruncate(fd, size) == -1) {
        printf("Erro ao definir o tamanho da memória partilhada.\n");
        exit(1);
    }

    void* ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Erro ao mapear a memória partilhada.\n");
        exit(1);
    }
 
    return ptr;
}

void destroy_shared_memory(char* name, void* ptr, int size) {
    if (munmap(ptr, size) == -1) {
        printf("Erro ao desmapear a memória partilhada.\n");
        exit(1);
    }

    if (shm_unlink(name) == -1) {
        printf("Erro ao remover a memória partilhada.\n");
        exit(1);
    }
}

void* allocate_dynamic_memory(int size) {
    void* ptr = malloc(size);
    
    if (ptr == NULL) {
        printf("Erro ao alocar memória dinâmica.\n");
        exit(1);
    }

    memset(ptr, 0, size);

    return ptr;
}

void deallocate_dynamic_memory(void* ptr) {
    if (ptr != NULL) {
        free(ptr);
        ptr = NULL;
    }
}

void write_main_patient_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad) {    
    // writeIndex: buffer->ptrs->in
    // readIndex: buffer->ptrs->out

    // if buffer is full
    if ((buffer->ptrs->in + 1) % buffer_size == buffer->ptrs->out) {
        printf("Buffer is full\n");
        return;
    }

    // copy ad to buffer
    memcpy(buffer->buffer + buffer->ptrs->in * sizeof(struct admission), ad, sizeof(struct admission));

    // increment write index
    buffer->ptrs->in = (buffer->ptrs->in + 1) % buffer_size;
}
void read_main_patient_buffer(struct circular_buffer* buffer, int patient_id, int buffer_size, struct admission* ad) {    
    // writeIndex: buffer->ptrs->in
    // readIndex: buffer->ptrs->out

    // if buffer is empty
    if (buffer->ptrs->out == buffer->ptrs->in) {
        ad->id = -1;

        return;
    }

    // get next admission
    struct admission* next_ad = (buffer->buffer + buffer->ptrs->out * sizeof(struct admission));

    //printf("<READING> next_ad->requesting_patient: %d\n", next_ad->requesting_patient);

    // if patient_id is not the same as the next admission
    if (next_ad->requesting_patient != patient_id) {
        ad->id = -1;

        return;
    }

    // copy ad from buffer
    memcpy(ad, buffer->buffer + buffer->ptrs->out * sizeof(struct admission), sizeof(struct admission));

    // increment read index
    buffer->ptrs->out = (buffer->ptrs->out + 1) % buffer_size;
}

void write_receptionist_doctor_buffer(struct circular_buffer* buffer, int buffer_size, struct admission* ad) {
    // writeIndex: buffer->ptrs->in
    // readIndex: buffer->ptrs->out

    // if buffer is full
    if ((buffer->ptrs->in + 1) % buffer_size == buffer->ptrs->out) {
        return;
    }

    // copy ex to buffer
    memcpy(buffer->buffer + buffer->ptrs->in * sizeof(struct admission), ad, sizeof(struct admission));

    // increment write index
    buffer->ptrs->in = (buffer->ptrs->in + 1) % buffer_size;
}

void read_receptionist_doctor_buffer(struct circular_buffer* buffer, int doctor_id, int buffer_size, struct admission* ad) {
    // writeIndex: buffer->ptrs->in
    // readIndex: buffer->ptrs->out

    // if buffer is empty
    if (buffer->ptrs->out == buffer->ptrs->in) {
        ad->id = -1;

        return;
    }

    // get next admission
    struct admission* next_ad = (buffer->buffer + buffer->ptrs->out * sizeof(struct admission));

    // if doctor_id is not the same as the next examination
    if (next_ad->requested_doctor != doctor_id) {
        ad->id = -1;

        return;
    }

    // copy ex from buffer
    memcpy(ad, buffer->buffer + buffer->ptrs->out * sizeof(struct admission), sizeof(struct admission));

    // increment read index
    buffer->ptrs->out = (buffer->ptrs->out + 1) % buffer_size;
}

void write_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad) {
  // check for free positions to write
    for (int i = 0; i < buffer_size; i++) {
        if (buffer->ptrs[i] == 0) {
            memcpy(buffer->buffer + i * sizeof(struct admission), ad, sizeof(struct admission));
            buffer->ptrs[i] = 1;
            return;
        }
    }
}


void read_patient_receptionist_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct admission* ad) {
    // check for occupied positions to read
    for (int i = 0; i < buffer_size; i++) {
        if (buffer->ptrs[i] == 1) {
            memcpy(ad, buffer->buffer + i * sizeof(struct admission), sizeof(struct admission));
            buffer->ptrs[i] = 0;
            return;
        }
    }

    // no admission
    ad->id = -1;
}
