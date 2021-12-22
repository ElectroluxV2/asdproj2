//
// Created by mate on 22.12.2021.
//

#ifndef ASDPROJ2_DECODE_H
#define ASDPROJ2_DECODE_H

#include "shared.h"

bool decode(const char* input, const char* output) {
    FILE* inputPointer = fopen(input, "rb");
    FILE* outputPointer = fopen(output, "w");
    if (inputPointer == NULL || outputPointer == NULL) return false;

    unsigned char encodedDictionaryLength;
    fread(&encodedDictionaryLength,sizeof(unsigned char),1,inputPointer);
    printf("encoded dictionary length: %d\n", encodedDictionaryLength);

    for (unsigned i = 0; i < encodedDictionaryLength; i++) {
        unsigned char key;
        unsigned char length;
        unsigned char value;

        fread(&key,sizeof(unsigned char),1,inputPointer);
        fread(&length,sizeof(unsigned char),1,inputPointer);
        fread(&value,sizeof(unsigned char),1,inputPointer);

        printf("K: %d, L: %d, V: %d => %c: |", key, length, value, key + ' ');
        for (int bit = length; bit; --bit) {  // count from length to 1
            putchar(value & (1 << (bit - 1)) ? '1' : '0');
        }
        printf("|\n");
    }

    unsigned char currentByte;
    while (fread(&currentByte, sizeof(unsigned char), 1, inputPointer) == 1) {
        for (int bit = 8; bit; --bit) {  // count from 8 to 1
//            putchar(currentByte & (1 << (bit - 1)) ? '1' : '0');
        }

//        printf(": %d\n", currentByte);
    }

    fclose(outputPointer);
    fclose(inputPointer);
    return true;
}

#endif //ASDPROJ2_DECODE_H
