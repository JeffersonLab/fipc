//
// Created by Vardan Gurjyan on 9/11/19.
//
#include "proto.h"

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int count = 100000000;
    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd < 0) {
        perror("shm_open()");
        return EXIT_FAILURE;
    }

    ftruncate(fd, SIZE);

    struct Data *data = (struct Data *)
            mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("request: mapped address: %p\n", data);

    for (int i = 0; i < NUM; ++i) {
        data->data[i] = i;
    }

    for (int i = 0; i < count; i++) {
        atomic_store(&data->state, 1);

        while (atomic_load(&data->state) != 2) {}
    }

    munmap(data, SIZE);

    close(fd);

    shm_unlink(NAME);

    return EXIT_SUCCESS;
}


