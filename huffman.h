//
// Created by mate on 22.12.2021.
//

#ifndef ASDPROJ2_HUFFMAN_H
#define ASDPROJ2_HUFFMAN_H

#include "shared.h"
#include "occurrencesCount.h"
#include "heap.h"
#include "queue.h"

// Total supported ascii keys
static const unsigned TOTAL_POSSIBLE_KEYS_IN_HUFFMAN_DICTIONARY = '~' - ' ' + 1 + 1; // +1 is for '\0' that is used in one-byte complement
// Amount we need to subtract from evey key to start indexing from 0 ak. int value of the lowest possible char key.
static const unsigned KEY_OFFSET_IN_HUFFMAN_DICTIONARY = ' ';

char** makeHuffmanDictionary() {
    return malloc(sizeof (char**) * TOTAL_POSSIBLE_KEYS_IN_HUFFMAN_DICTIONARY);
}

char** getValueFromHuffmanDirectoryByIndex(unsigned index, char** dictionary) {
    return dictionary + index;
}

void freeHuffmanDictionary(char** dictionary) {
    for (unsigned i = 0; i < TOTAL_POSSIBLE_KEYS_IN_HUFFMAN_DICTIONARY; i++) {
        free(*getValueFromHuffmanDirectoryByIndex(i, dictionary));
    }
}

char** getValueFromHuffmanDirectory(char key, char** dictionary) {
    if (key == '\0') return getValueFromHuffmanDirectoryByIndex(TOTAL_POSSIBLE_KEYS_IN_HUFFMAN_DICTIONARY - 1, dictionary);
    return getValueFromHuffmanDirectoryByIndex(key - KEY_OFFSET_IN_HUFFMAN_DICTIONARY, dictionary);
}

void printHuffmanDirectory(char** huffmanDictionary) {
    for (unsigned i = 0; i < TOTAL_POSSIBLE_KEYS_IN_HUFFMAN_DICTIONARY; i++) {
        char key = (char) (' ' + i);
        printf("%c: %s\n", key, *getValueFromHuffmanDirectory(key, huffmanDictionary));
    }
}

char** getHuffmanDictionaryForFile(const char* file) {
    unsigned long long* occurrences = countOccurrencesOfCharactersInFile(file);
    // n
    unsigned long long totalUniqueCharacters = countTotalNonZeroOccurrences(occurrences) + 1;
    // There always will be 2n - 1 output groups
    unsigned long long totalPossibleGroupsInStore = totalUniqueCharacters + totalUniqueCharacters - 1;

    // First n groups will be created during counting phase
    unsigned long long lastFreeGroupsIndex = 0;
    group* groups = makeGroupsFromOccurrences(occurrences, totalPossibleGroupsInStore, &lastFreeGroupsIndex);
    free(occurrences);

    // Our queue will never exceed n
    group** queue = malloc(sizeof(group*) * totalUniqueCharacters);
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

    char** dictionary = makeHuffmanDictionary();

    // Decode bit values for first n groups
    for (unsigned long long index = 0; index < totalUniqueCharacters; index++) {
        group* currentGroup = groups + index;
        char characterToBeEncoded = *currentGroup->value;

        *(getValueFromHuffmanDirectory(characterToBeEncoded, dictionary)) = calloc(sizeof(char), 10); // TODO: better way of estimating how many bits can be
        char* currentValueInDictionary = *(getValueFromHuffmanDirectory(characterToBeEncoded, dictionary));

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
