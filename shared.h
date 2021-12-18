//
// Created by mate on 16.12.2021.
//

#ifndef ASDPROJ2_SHARED_H
#define ASDPROJ2_SHARED_H

#include <malloc.h>
#include <memory.h>
#include <stdbool.h>

typedef struct group {
    char* value;
    unsigned long long count;
    struct group* parent;
    bool isRightChild;
} group;

static inline void swap(void* first, void* second) {
    size_t size = sizeof(first);
    void* tmp = malloc(size);
    memcpy(tmp, first, size);
    memcpy(first, second, size);
    memcpy(second, tmp, size);
    free(tmp);
}

//static inline void swap(group** first, group** second) {
//    group* tmp = *first;
//    *first = *second;
//    *second = tmp;
//}

static char* combineStrings(const char* first, const char* second) {
    char* combined = malloc(sizeof(first) + sizeof(second));
    strcpy(combined, first);
    strcat(combined, second);
    return combined;
}

//char* stringReverse(char *string) {
//    for (unsigned long i = strlen(string) - 1, j = 0; i > j; i--, j++) {
////        const char tmp = *(string + i);
////        *(string + i) = *(string + j);
////        *(string + j) = tmp;
//        swap(string + i, string + j);
//    }
//
//    return string;
//}

void stringReverse(char *string) {
    unsigned int length = strlen(string);
    for (int i = 0; i < length / 2; i++) {
        char tmp = *(string + i);
        *(string + i) = *(string + length - 1 - i);
        *(string + length - 1 - i) = tmp;
    }
}

#endif //ASDPROJ2_SHARED_H
