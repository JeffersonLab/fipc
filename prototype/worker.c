//
// Created by Vardan Gurjyan on 9/11/19.
//
#include "protocol.h"

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>

int main() {

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

    ftruncate(fd, SIZE);

    struct Data *data = (struct Data *)
            mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("worker: mapped address: %p\n", data);

    clock_t begin = clock();
    for (int i = 0; i < count; i++) {
        while (atomic_load(&data->state) != 1) {}

        //for(int i = 0 ; i < 300; i++) {
        //data->data[i] = data->data[i] * 33;
        //}
        atomic_store(&data->state, 2);

    }
    clock_t end = clock();

    time_spent += (double) (end - begin) / CLOCKS_PER_SEC;

    printf("Time elpased is %f seconds\n", time_spent);

    long double r = count / time_spent;

    printf("Transfer rate =  %Lf Hz\n", r);


    munmap(data, SIZE);

    close(fd);

    return EXIT_SUCCESS;
}

