//
// Created by mate on 16.12.2021.
//

#ifndef ASDPROJ2_SHARED_H
#define ASDPROJ2_SHARED_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

typedef struct group {
    char* value;
    unsigned long long count;
    struct group* parent;
    bool isRightChild;
} group;

static inline void swap(group** first, group** second) {
    group* tmp = *first;
    *first = *second;
    *second = tmp;
}

static char* combineStrings(const char* first, const char* second) {
    char* combined = malloc(sizeof(first) + sizeof(second));
    strcpy(combined, first);
    strcat(combined, second);
    return combined;
}


void stringReverse(char *string) {
    unsigned int length = strlen(string);
    for (int i = 0; i < length / 2; i++) {
        char tmp = *(string + i);
        *(string + i) = *(string + length - 1 - i);
        *(string + length - 1 - i) = tmp;
    }
}

#endif //ASDPROJ2_SHARED_H
