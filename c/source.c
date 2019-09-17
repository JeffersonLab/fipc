//
// Created by Vardan Gurjyan on 9/17/19.
//
#include "fipc.h"

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

void setConsumers(int i) {
    nConsumers = i;
}

int main() {
    int count = 100000000;

    int myId = 0;
    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror(NAME);
        return EXIT_FAILURE;
    }

    ftruncate(fd, DATA_SIZE);
    struct Data *data = (struct Data *)
            mmap(0, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("source: mapped address: %p\n", data);

    // Populate shared memory with data blocks. Blocks are equal size (defined by BLOCK_SIZE)
    // There are N_THREADS blocks in the memory
    for (int j = 0; j < N_THREADS; j++) {
        for (int jj = 0; jj < BLOCK_SIZE; jj++) {
            data->data[j][jj] = jj;
        }
    }

    // Main loop. This will become loop for ever that will consume entire file or DAQ source.
    for (int i = 0; i < count; i++) {
        atomic_store(&data->block_id_flag, -1);
        while (atomic_load(&data->block_id_flag) == -1) {}

        // Get index in the shared memory data array where data was consumed
        int thread_index = atomic_load(&data->block_id_flag);

        // check my connected service, i.e. consumers
        int c_status = atomic_load(&data->S_Flags[myId]);
        if (c_status == nConsumers) {
            if (thread_index > 0) {

                // Update data at the thread/evt index, since data in here is already processed
                for (int j = 0; j < BLOCK_SIZE; j++) {
                    data->data[thread_index][j] = j;
                }
            }
            atomic_store(&data->S_Flags[myId], 0);
            atomic_store(&data->block_id_flag, -1);
        }
    }

    munmap(data, DATA_SIZE);
    close(fd);
    shm_unlink(NAME);
    return EXIT_SUCCESS;
}

