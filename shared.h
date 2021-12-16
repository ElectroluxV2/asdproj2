//
// Created by mate on 16.12.2021.
//

#ifndef ASDPROJ2_SHARED_H
#define ASDPROJ2_SHARED_H

#include <malloc.h>
#include <memory.h>

typedef struct group {
    char* value;
    unsigned long long count;
} group;


//static inline void swap(void* first, void* second) {
//    size_t size = sizeof(first);
//    void* tmp = malloc(size);
//    memcpy(tmp, first, size);
//    memmove(first, second, size);
//    memmove(second, tmp, size);
//    free(tmp);
//}

static inline void swap(group** first, group** second) {
    group* tmp = *first;
    *first = *second;
    *second = tmp;
}

static inline char* combineStrings(const char* first, const char* second) {
    char* combined = malloc(sizeof(first) + sizeof(second));
    strcpy(combined, first);
    strcat(combined, second);
    return combined;
}


#endif //ASDPROJ2_SHARED_H
