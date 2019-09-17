//
// Created by Vardan Gurjyan on 9/17/19.
//

#ifndef FIPC_FIPC_H
#define FIPC_FIPC_H

#include <stdatomic.h>

#define N_SERVICES 7
#define N_THREADS 4
#define BLOCK_SIZE 3000

struct Data {
    atomic_int block_id_flag;
    atomic_int S_Flags[N_SERVICES];
    int data[N_THREADS][BLOCK_SIZE];
};

int nConsumers = 1;

#define DATA_SIZE sizeof(struct Data)
#define NAME "/fipc_shm"


#endif //FIPC_FIPC_H
