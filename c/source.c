//
// Created by Vardan Gurjyan on 9/17/19.
//
#include "fipc.h"

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>

#include <fcntl.h>


int main() {

    // Flag for gracefully exiting the program
    bool s_exit = false;

    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror(NAME);
        return EXIT_FAILURE;
    }

    ftruncate(fd, DATA_SIZE);
    struct Data *data = (struct Data *)
            mmap(0, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("source: mapped address: %p\n", data);

    // Populate shared memory with data blocks. Blocks are equal size (defined by BLOCK_SIZE).
    // There are N_BLOCKS blocks in the memory
    for (int i = 0; i < N_BLOCKS; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            data->data[i][j] = j;
        }
        data->B_Flags[i] = 0;
    }

    // Main loop. This will become loop for ever that will consume entire file or DAQ source.
    while (s_exit == false) {

        // Check the B_Flags to see which buffer is already processed
        for (int k = 0; k < N_BLOCKS; k++) {
            int block_index = atomic_load(&data->B_Flags[k]);
            if (block_index < 0) {
                s_exit = true;
            } else if (block_index > 0) {

                // Update block at the block_index, since data in here is already processed
                for (int l = 0; l < BLOCK_SIZE; l++) {
                    data->data[block_index][l] = l;
                }
                atomic_store(&data->B_Flags[block_index], 0);
            }
        }

    }

    munmap(data, DATA_SIZE);
    close(fd);
    shm_unlink(NAME);
    return EXIT_SUCCESS;
}