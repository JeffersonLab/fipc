//
// Created by Vardan Gurjyan on 9/17/19.
//


#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#include "fipc.h"

int main() {

    int chain_id;
    int chain_next_id;

    printf("Enter service position in the composition \n");
    scanf("%d", &chain_id);

    printf("Enter linked service position \n");
    scanf("%d", &chain_next_id);


    double time_spent = 0.0;
    int count = 100000000;

    int fd = -1;
    while (fd == -1) {
        fd = shm_open(NAME, O_RDWR, 0666);
        if (fd < 0 && errno != ENOENT) {
            perror("shm_open()");
            return EXIT_FAILURE;
        }
    }

    ftruncate(fd, DATA_SIZE);

    struct Data *data = (struct Data *)
            mmap(0, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("worker-service: mapped address: %p\n", data);

    clock_t begin = clock();
    for (int i = 0; i < count; i++) {
        if (chain_id == 0) {

            //@todo this service is looking into the first block. Make it any available block
            for (int j = 0; j < N_BLOCKS; j++) {
                if (&data->B_Flags[j] == 0) {
                    // @todo perform buffer processing here
                    if (chain_next_id == 0) {
                        atomic_store(&data->B_Flags[j], 1);
                    } else {
                        atomic_store(&data->S_Flags[chain_id], j);
                    }
                    break;
                }
            }
        } else {
            while (atomic_load(&data->S_Flags[chain_id] - 1) < 0) {}
            int data_id = atomic_load(&data->S_Flags[chain_id] - 1);
            // @todo perform buffer processing here
            if (chain_next_id == 0) {
                atomic_store(&data->B_Flags[data_id], 1);
            }
            atomic_store(&data->S_Flags[chain_id], data_id);
            atomic_store(&data->S_Flags[chain_id-1], -1);
        }
    }

    // Tell source service to exit
    atomic_store(&data->B_Flags[0], -1);
    clock_t end = clock();

    time_spent += (double) (end - begin) / CLOCKS_PER_SEC;

    printf("Time elapsed is %f seconds\n", time_spent);

    long double r = count / time_spent;

    printf("Transfer rate =  %Lf Hz\n", r);

    munmap(data, DATA_SIZE);
    close(fd);
    return EXIT_SUCCESS;
}

