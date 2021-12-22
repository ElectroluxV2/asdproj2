//
// Created by mate on 22.12.2021.
//

#ifndef ASDPROJ2_ENCODE_H
#define ASDPROJ2_ENCODE_H

#include "shared.h"
#include "huffman.h"

bool encodeWithDictionary(const char* input, const char* output, char** huffmanDictionary) {
    FILE* inputPointer = fopen(input, "r");
    FILE* outputPointer = fopen(output, "wb");
    if (inputPointer == NULL || outputPointer == NULL) return false;

    // Encode dictionary for decoding
    unsigned char nonNullKeys = countNonNullValuesInHuffmanDirectory(huffmanDictionary);
    printf("Non null keys: %d\n", nonNullKeys);

    // First byte will always be dictionary count
    fputc(nonNullKeys, outputPointer); // n

    unsigned char longestBitLength = getLongestBitLength(huffmanDictionary);
    // Next there will be 2n pairs in which even item will be key and odd one will be value
    for (unsigned char i = 0; i < TOTAL_POSSIBLE_KEYS_IN_HUFFMAN_DICTIONARY; i++) {
        const char* value = *getValueFromHuffmanDirectoryByIndex(i, huffmanDictionary);
        if (value == NULL) continue;

        // Flush key
        fputc(i, outputPointer);

        // Flush value original length
        fputc((unsigned char) strlen(value), outputPointer);

        unsigned char valueAsNumber = strtol(value, NULL, 2);
        printf("K: %d, V: %d\n", i, valueAsNumber);

        // Flush value
        fputc(valueAsNumber, outputPointer);
    }

    // Now encode file
    // TODO: This could be more performant if a stack of characters was used instead
    char* currentBytesString = calloc(sizeof(char), LONGEST_HUFFMAN_BIT_CODE * 2 + 1); // +1 for \0
    char currentChar;

    while ((currentChar = (char) fgetc(inputPointer)) != EOF) {
        // Get huffman bit code for current character
        const char* currentBitCode = *getValueFromHuffmanDirectory(currentChar, huffmanDictionary);
        // Concat with current bytes complement
        strcat(currentBytesString, currentBitCode);

        if (strlen(currentBytesString) < 8) continue; // Concat more data in order to flush to drive

//        printf("total: %s", currentBytesString);

        // 8 is byte +1 for trailing null
        char* singleByteString = calloc(sizeof(char), 9);
        // Remove first 8 chars from string and flush tem as binary number to output
        strncpy(singleByteString, currentBytesString, 8 * sizeof(char));
        // Move other part to the beginning
        strcpy(currentBytesString, currentBytesString + 8 * sizeof(char));

        // Flush singleByteString as binary number
//        printf(", byte: %s, rest: %s\n", singleByteString, currentBytesString);
        unsigned char byteStingAsNumber = strtol(singleByteString, NULL, 2);
        fputc(byteStingAsNumber, outputPointer);

        free(singleByteString);
    }

    // If anything left in currentBytesString that is shorter than 8 chars
    // we need to add \0 at the end and then
    // complement that string with '0' until currentBytesString will be exactly 2 bytes
    if (strlen(currentBytesString) != 0) {
//        printf("before complement: %s\n", currentBytesString);
        strcat(currentBytesString,*getValueFromHuffmanDirectory('\0', huffmanDictionary));
//        printf("after complement to first byte: %s\n", currentBytesString);

        while (strlen(currentBytesString) % 8 != 0) {
            strcat(currentBytesString, "0");
        }

//        printf("after complement to second byte: %s\n", currentBytesString);

        // Flush reaming 2 bytes as numbers
        char* firstPart = calloc(sizeof(char), 9); // +1 for \0
        char* secondPart = calloc(sizeof(char), 9); // +1 for \0

        strncpy(firstPart, currentBytesString, 8 * sizeof(char));
        *(firstPart + 9) = '\0';

        strncpy(secondPart, currentBytesString + 8 * sizeof(char), 8 * sizeof(char));
        *(secondPart + 9) = '\0';

        unsigned char firstNumber = strtol(firstPart, NULL, 2);
        unsigned char secondNumber = strtol(secondPart, NULL, 2);

        fputc(firstNumber, outputPointer);
        fputc(secondNumber, outputPointer);

        free(firstPart);
        free(secondPart);
    }

    free(currentBytesString);

    fclose(outputPointer);
    fclose(inputPointer);
    return true;
}

bool encode(const char* input, const char* output) {
    char** huffmanDictionary = getHuffmanDictionaryForFile(input);
    printHuffmanDirectory(huffmanDictionary);

    bool status = encodeWithDictionary(input, output, huffmanDictionary);

    freeHuffmanDictionary(huffmanDictionary);
    free(huffmanDictionary);
    return status;
}

#endif //ASDPROJ2_ENCODE_H
