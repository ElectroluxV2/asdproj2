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

    char** huffmanDictionary = makeHuffmanDictionary();

    for (unsigned i = 0; i < encodedDictionaryLength; i++) {
        unsigned char key;
        unsigned char length;
        unsigned char value;

        fread(&key,sizeof(unsigned char),1,inputPointer);
        fread(&length,sizeof(unsigned char),1,inputPointer);
        fread(&value,sizeof(unsigned char),1,inputPointer);

        *(getValueFromHuffmanDirectoryByIndex(key, huffmanDictionary)) = calloc(sizeof(char), length + 1); // +1 for \0
        char* currentValueInDictionary = *(getValueFromHuffmanDirectoryByIndex(key, huffmanDictionary));

        char* bitString = malloc(sizeof(char) * 2);
        *(bitString + 1) = '\0';
        for (int bit = length; bit; --bit) {  // count from length to 1
            *bitString = value & (1 << (bit - 1)) ? '1' : '0';
            strcat(currentValueInDictionary, bitString);
        }
//        printf("K: %d, L: %d, V: %d => %c: |%s|\n", key, length, value, key + ' ', currentValueInDictionary);
        free(bitString);
    }

    printHuffmanDirectory(huffmanDictionary);

    unsigned char currentByte;
    char* currentByteString = calloc(sizeof(char),9);
    char* allBytesString = calloc(sizeof(char),LONGEST_HUFFMAN_BIT_CODE * 2);
    bool stopReading = false;
    while (fread(&currentByte, sizeof(unsigned char),1,inputPointer) == 1 && !stopReading) {
        byteToString(currentByte, currentByteString);
        strcat(allBytesString, currentByteString);

        printf("Before any match: |%s|\n", allBytesString);

        for (int i = 1; i <= strlen(allBytesString); i++) {
            char* searchValue = calloc(sizeof(char), i + 1);
            *(searchValue + i) = '\0';
            strncpy(searchValue, allBytesString, i);

            const char decodedKey = getCharacterFromBitCode(huffmanDictionary, searchValue);

            printf("Searching: |%s|\n", searchValue);

            if (decodedKey == -1) continue;
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



//    char* bytesString = calloc(sizeof(char), 2 * LONGEST_HUFFMAN_BIT_CODE);
//    unsigned char currentByte;
//    bool stopReading = false;
//    while (fread(&currentByte, sizeof(unsigned char), 1, inputPointer) == 1 && !stopReading) {
//
//        char* bitString = calloc(sizeof(char), 2);
//        *(bitString + 1) = '\0';
//        for (int bit = 8; bit; --bit) {  // count from 8 to 1
//            *bitString = currentByte & (1 << (bit - 1)) ? '1' : '0';
//            strcat(bytesString, bitString);
//        }
//        free(bitString);
//
//        printf("BT: |%s|\n", bytesString);
//        //
//        for (unsigned i = 1; i <= strlen(bytesString); i++) {
//            printf("BT: |%s|\n", bytesString);
//
//            char* currentPart = calloc(sizeof(char), i + 2);
//            strncpy(currentPart, bytesString, i);
//            *(currentPart + i + 1) = '\0';
//
//            printf("Current part to find: (%d) |%s|\n", i, currentPart);
//            const char key = getCharacterFromBitCode(huffmanDictionary, currentPart);
//
//            if (key == '\0') {
//                printf("Found end of data\n");
//                stopReading = true;
//                break;
//            }
//            if (key == -1) continue;
//
//            // Flush key to output
//            fputc(key,outputPointer);
//            printf("Decoded char: %c\n", key);
//
//            // Remove from 0 to i chars from bytesString and move rest at the beginning
//            printf("BT: |%s|\n", bytesString);
//            strcpy(bytesString, bytesString + i);
//            printf("BT: |%s|\n", bytesString);
//
//            i = 0;
//
//            free(currentPart);
//        }
//
//        printf("Append next byte\n");
//    }
//
//    free(bytesString);

    freeHuffmanDictionary(huffmanDictionary);
    free(huffmanDictionary);

    fclose(outputPointer);
    fclose(inputPointer);
    return true;
}

#endif //ASDPROJ2_DECODE_H
