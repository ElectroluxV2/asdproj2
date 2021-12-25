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

    byte encodedDictionaryLength;
    fread(&encodedDictionaryLength,sizeof(byte),1,inputPointer);
    printf("encoded dictionary length: %d\n", encodedDictionaryLength);

    char** huffmanDictionary = calloc(sizeof(char*), TOTAL_POSSIBLE_ASCII_KEYS);

    for (byte i = 0; i < encodedDictionaryLength; i++) {
        byte readKey;
        byte readLength;
        byte readValue;

        fread(&readKey, sizeof(byte), 1, inputPointer);
        fread(&readLength, sizeof(byte), 1, inputPointer);
        fread(&readValue, sizeof(byte), 1, inputPointer);

        *(huffmanDictionary + readKey) = calloc(sizeof(char), readLength + 1); // +1 for \0
        char* currentValueInDictionary = *(huffmanDictionary + readKey);

        char* bitString = calloc(sizeof(char), 2);
        *(bitString + 1) = '\0';
        for (int bit = readLength; bit; --bit) {  // count from readLength to 1
            *bitString = readValue & (1 << (bit - 1)) ? '1' : '0';
            strcat(currentValueInDictionary, bitString);
        }
//        printf("K: %d, L: %d, V: %d => %c: |%s|\n", readKey, readLength, readValue, readKey + ' ', currentValueInDictionary);
        free(bitString);
    }

    printHuffmanDirectory(huffmanDictionary);

    byte currentByte;
    char* currentByteString = calloc(sizeof(char),9);
    char* allBytesString = calloc(sizeof(char),LONGEST_HUFFMAN_BIT_CODE * 2);
    bool stopReading = false;
    while (fread(&currentByte, sizeof(byte),1,inputPointer) == 1 && !stopReading) {
        byteToString(currentByte, currentByteString);
        strcat(allBytesString, currentByteString);

        printf("Before any match: |%s|\n", allBytesString);

        for (int i = 1; i <= strlen(allBytesString); i++) {
            char* searchValue = calloc(sizeof(char), i + 1);
            *(searchValue + i) = '\0';
            strncpy(searchValue, allBytesString, i);

            char decodedKey;
            if (!getKeyFromBitCode(huffmanDictionary, searchValue, &decodedKey)) continue;

            printf("Searching: |%s|\n", searchValue);

            if (decodedKey == '\0') {
                printf("Found end code\n");
                stopReading = true;
                break;
            }

            printf("Searching: |%s| found key: |%c|\n", searchValue, decodedKey);
            fputc(decodedKey, outputPointer);

            printf("Before string substr: |%s| -(%s)\n", allBytesString, searchValue);
            memmove(allBytesString, allBytesString + i, strlen(allBytesString) - i + 1);
            printf("After string substr: |%s|\n", allBytesString);
            i = 0;
            free(searchValue);
        }

        printf("After any match: |%s|\n", allBytesString);

    }

    free(allBytesString);
    free(currentByteString);

    freeHuffmanDictionary(huffmanDictionary);
    free(huffmanDictionary);

    fclose(outputPointer);
    fclose(inputPointer);
    return true;
}

#endif //ASDPROJ2_DECODE_H
