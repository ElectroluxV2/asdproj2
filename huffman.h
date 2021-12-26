//
// Created by mate on 22.12.2021.
//

#ifndef ASDPROJ2_HUFFMAN_H
#define ASDPROJ2_HUFFMAN_H

#include "shared.h"
#include "occurrencesCount.h"
#include "heap.h"
#include "queue.h"

static const unsigned LONGEST_HUFFMAN_BIT_CODE = 8; // unsigned char

void freeHuffmanDictionary(char** dictionary) {
    for (char key = 0; key < TOTAL_POSSIBLE_ASCII_KEYS; key++) {
        free(*(dictionary + key));
    }
}

void printHuffmanDictionary(char** huffmanDictionary) {
    for (char key = 0; key < TOTAL_POSSIBLE_ASCII_KEYS; key++) {
        const char* value = *(huffmanDictionary + key);
        if (value == NULL) continue;
        printf("%c: %s\n", key, value);
    }
}

byte countNonNullValuesInHuffmanDirectory(char** dictionary) {
    byte counter = 0;
    for (char key = 0; key < TOTAL_POSSIBLE_ASCII_KEYS; key++) {
        if (*(dictionary + key) == NULL) continue;
        counter++;
    }
    return counter;
}

bool getKeyFromBitCode(char** huffmanDictionary, const char* searchValue, char* foundKey) {
    for (char key = 0; key < TOTAL_POSSIBLE_ASCII_KEYS; key++) {
        const char* value = *(huffmanDictionary + key);
        if (value == NULL) continue;

        if (strcmp(searchValue, value) != 0) continue;

        *foundKey = key;
        return true;
    }

    return false;
}

byte getLongestBitLength(char** dictionary) {
    byte max = 0;
    for (char key = 0; key < TOTAL_POSSIBLE_ASCII_KEYS; key++) {
        char* bitCode = *(dictionary + key);
        if (bitCode == NULL) continue;
        byte current = strlen(bitCode);
        if (current < max) continue;
        max = current;
    }
    return max;
}

char** getHuffmanDictionaryForFile(const char* file) {
    unsigned long long* occurrences = countOccurrencesOfCharactersInFile(file);
    // n
    unsigned long long totalUniqueCharacters = countTotalNonZeroOccurrences(occurrences) + 1; // + 1 for \0
    // There always will be 2n - 1 output groups
    unsigned long long totalPossibleGroupsInStore = totalUniqueCharacters + totalUniqueCharacters - 1;

    // First n groups will be created during counting phase
    unsigned long long lastFreeGroupsIndex = 0;
    group* groups = makeGroupsFromOccurrences(occurrences, totalPossibleGroupsInStore, &lastFreeGroupsIndex);
    free(occurrences);

    // Our queue will never exceed n
    group** queue = calloc(sizeof(group*), totalUniqueCharacters);
    unsigned long long currentQueueLength = totalUniqueCharacters;

    // Insert pointers to groups
    for (unsigned long long index = 0; index < totalUniqueCharacters; index++) {
        *(queue + index) = groups + index;
    }

    // Every queue must be built on top of heap
    buildHeap(queue, currentQueueLength);

    // Next n - 1 groups are calculated here and settled at next free index starting from n
    for (unsigned long long i = 0; i < totalUniqueCharacters - 1; i++) {
        group* left = extract(queue, &currentQueueLength);
        group* right = extract(queue, &currentQueueLength);

        //printf("left: %s:%lld, right: %s:%lld\n", (char*) left->value, left->count, (char*) right->value, right->count);

        group* pointerToLatestFreeGroup = (groups + lastFreeGroupsIndex++);
        pointerToLatestFreeGroup->value = combineStrings(left->value, right->value);
        pointerToLatestFreeGroup->count = left->count + right->count;

        left->parent = pointerToLatestFreeGroup;
        left->isRightChild = false;

        right->parent = pointerToLatestFreeGroup;
        right->isRightChild = true;

        insert(queue, &currentQueueLength, pointerToLatestFreeGroup);
    }

    // No more needed
    free(queue);

    char** dictionary = calloc(sizeof (char**), TOTAL_POSSIBLE_ASCII_KEYS);

    // Decode bit values for first n groups
    for (unsigned long long index = 0; index < totalUniqueCharacters; index++) {
        group* currentGroup = groups + index;
        char characterToBeEncoded = *currentGroup->value;

        *(dictionary + characterToBeEncoded) = calloc(sizeof(char), LONGEST_HUFFMAN_BIT_CODE);
        char* currentValueInDictionary = *(dictionary + characterToBeEncoded);

        // This will get byte value in reversed order
        while (currentGroup->parent != NULL) {
            strcat(currentValueInDictionary, currentGroup->isRightChild ? "1" : "0");
            currentGroup = currentGroup->parent;
        }

        stringReverse(currentValueInDictionary);
        //printf("Character: %c byteValue: %s\n", characterToBeEncoded, *(dictionary + dictionaryIndex));
    }

    for (unsigned long long index = 0; index < totalPossibleGroupsInStore; index++) {
        free((groups + index)->value);
    }

    free(groups);
    return dictionary;
}

#endif //ASDPROJ2_HUFFMAN_H
