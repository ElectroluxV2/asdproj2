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
    memmove(first, second, size);
    memmove(second, tmp, size);
    free(tmp);
}

//static inline void swap(group** first, group** second) {
//    group* tmp = *first;
//    *first = *second;
//    *second = tmp;
//}

static inline char* combineStrings(const char* first, const char* second) {
    char* combined = malloc(sizeof(first) + sizeof(second));
    strcpy(combined, first);
    strcat(combined, second);
    return combined;
}

char* stringReverse(char *str) {
    char *p1, *p2;

    if (! str || ! *str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

#endif //ASDPROJ2_SHARED_H
