cmake_minimum_required(VERSION 3.14)
project(fipc C)

set(CMAKE_C_STANDARD 11)

add_executable(fipc c/fipc.h c/source.c c/service.c c/sink.c prototype/proto.h prototype/producer.c prototype/consumer.c)