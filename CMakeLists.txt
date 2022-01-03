cmake_minimum_required(VERSION 3.21)
project(asdproj2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS_RELEASE "-Wall -Wextra")
set(CMAKE_C_FLAGS_RELEASE "-O3")
add_executable(asdproj2 main.c heap.h shared.h queue.h occurrencesCount.h huffman.h encode.h decode.h)
