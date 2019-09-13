//
// Created by Vardan Gurjyan on 9/11/19.
//

#ifndef FIPC_FIPC_H
#define FIPC_FIPC_H

#include <stdatomic.h>

struct Data {
    atomic_int state;
    int data[];
};

#define NAME "/shmem-example"

#define NUM 30000

#define SIZE (sizeof(struct Data) + NUM * sizeof(int))

#endif //FIPC_FIPC_H
