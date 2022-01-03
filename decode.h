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
//    printf("encoded dictionary length: %d\n", encodedDictionaryLength);

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

//    printHuffmanDictionary(huffmanDictionary);

    // Represents currently read byte
    byte currentByte;
    // This String will contain at most 8 characters. It's string view of byte. It will consist only of '0'os and '1'ones.
    char* currentByteString = calloc(sizeof(char),9);
    // This String will contain at most 16 characters. Rest of previous byte and currently read byte ak. "current"
    char* allBytesString = calloc(sizeof(char),(LONGEST_HUFFMAN_BIT_CODE * 2) + 1);
    //
    unsigned currentAllBytesStringLength = 0;
    // This enables us to break outer loop by setting value to false
    bool stopReading = false;

    //
    unsigned currentGuessLength = 1;
    //
    char* currentGuess = calloc(sizeof(char), LONGEST_HUFFMAN_BIT_CODE + 1);


    // Decoding single characters, we can only read file byte by byte
    // This loop will read all reaming bytes from input file
    // This loop may be terminated before reaching end of file without any special care
    while (fread(&currentByte, sizeof(byte),1,inputPointer) == 1 && !stopReading) {
        if (currentAllBytesStringLength > 16) {
            fprintf(stderr, "MEMORY SAFETY CHECK ERROR (currentAllBytesStringLength)\n");
            break;
        }

        byteToString(currentByte, currentByteString);
//        printf("|%s|: %d\n", allBytesString, currentAllBytesStringLength);

        memcpy(allBytesString + currentAllBytesStringLength, currentByteString, 8);
        currentAllBytesStringLength += 8;
        *(allBytesString + currentAllBytesStringLength + 1) = '\0';

        while (currentGuessLength < currentAllBytesStringLength) {
            if (currentGuessLength > LONGEST_HUFFMAN_BIT_CODE) {
                stopReading = true;
                fprintf(stderr, "MEMORY SAFETY CHECK ERROR (currentGuessLength)\n");
                break;
            }

            memcpy(currentGuess, allBytesString, currentGuessLength);
            *(currentGuess + currentGuessLength) = '\0';

//            printf("Current guess: |%s|: %d\n", currentGuess, currentGuessLength);

            char foundKey = ' ';
            if (!getKeyFromBitCode(huffmanDictionary, currentGuess, &foundKey)) {
                currentGuessLength++;
                continue;
            }

//            printf("Found key: |%c|\n", foundKey);

            if (foundKey == '\0') {
                break;
                stopReading = true;
            }

            fputc(foundKey, outputPointer);

            currentAllBytesStringLength -= currentGuessLength;
            memmove(allBytesString, allBytesString + currentGuessLength, currentAllBytesStringLength);
            *(allBytesString + currentAllBytesStringLength + 1) = '\0';

            currentGuessLength = 1;
        }
    }

    free(currentGuess);

    freeHuffmanDictionary(huffmanDictionary);
    free(huffmanDictionary);

    fclose(outputPointer);
    fclose(inputPointer);
    return true;
}

#endif //ASDPROJ2_DECODE_H
