cmake_minimum_required(VERSION 3.14)
project(fipc C)

set(CMAKE_C_STANDARD 11)

add_executable(fipc prototype/fipc.h prototype/producer.c prototype/consumer.c)