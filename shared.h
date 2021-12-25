//
// Created by mate on 16.12.2021.
//

#ifndef ASDPROJ2_SHARED_H
#define ASDPROJ2_SHARED_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

static const unsigned TOTAL_POSSIBLE_ASCII_KEYS = 256;

typedef struct group {
    char* value;
    unsigned long long count;
    struct group* parent;
    bool isRightChild;
} group;

typedef unsigned char byte;

static inline void swap(group** first, group** second) {
    group* tmp = *first;
    *first = *second;
    *second = tmp;
}

static char* combineStrings(const char* first, const char* second) {
    char* combined = calloc(sizeof(char), strlen(first) + strlen(second) + 1);
    strcpy(combined, first);
    strcat(combined, second);
    return combined;
}

static void stringReverse(char* string) {
    unsigned int length = strlen(string);
    for (int i = 0; i < length / 2; i++) {
        char tmp = *(string + i);
        *(string + i) = *(string + length - 1 - i);
        *(string + length - 1 - i) = tmp;
    }
}

static char* byteToString(const byte byte, char* bitString) {
    *(bitString + 8) = '\0';
    for (int bit = 8; bit; --bit) {  // count from 8 to 1
        *(bitString + bit - 1) = byte & (1 << (bit - 1)) ? '1' : '0';
    }
    stringReverse(bitString);
    return bitString;
}

static char* convertCharacterToString(char character) {
    char* dynamic = malloc(sizeof(char) * 2);
    *dynamic = character;
    *(dynamic + 1) = '\0';
    return dynamic;
}




#endif //ASDPROJ2_SHARED_H
