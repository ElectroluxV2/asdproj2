#include <stdlib.h>
#include "shared.h"
#include "heap.h"
#include "queue.h"
#include "occurrencesCount.h"

void addGroupTBD(group* array, const unsigned long long index, char* value, const unsigned long long count) {
    group created = {
            value,
            count
    };
    *(array + index) = created;
}

char* createDynamicStringTBD(const char* str) {
    char* dynamic = malloc(sizeof(str));
    strcpy(dynamic, str);
    return dynamic;
}

unsigned long long calculateSizeOfEncodedInput(char** dictionary, FILE* input) {
    unsigned long long size = 0;

    fseek(input, 0, SEEK_END);
    fseek(input, 0, SEEK_SET);

    int c;
    while ((c = fgetc(input)) != EOF) {
        unsigned int dictionaryIndex = 0;
        dictionaryIndex = c - 'a';
        size += strlen(*(dictionary + dictionaryIndex));
    }
    size += strlen(*(dictionary + 'z' - 'a' + 1));
    return size + (8 - size%8);
}

void tryToMatchStringWithDictionary(char** dictionary, const char* string, unsigned long long* index) {
    for (int i = 0; i < 'z' - 'a' + 1; i++) {
        if (*(dictionary + i) == string) {
            *index = i;
            return;
        }
    }
    index = NULL;
}

int main() {
    unsigned long long* occurrences = countOccurrencesOfCharactersInFile("./../input");
    // n
    unsigned long long totalUniqueCharacters = countNonZero(occurrences) + 1;
    // There always will be 2n - 1 output groups
    unsigned long long totalPossibleGroupsInStore = totalUniqueCharacters + totalUniqueCharacters - 1;

    // First n groups will be created during counting phase
    unsigned long long lastFreeGroupsIndex = 0;
    group* groups = getGroupsFromOccurrences(occurrences, totalPossibleGroupsInStore, &lastFreeGroupsIndex);
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

    char** dictionary = malloc(sizeof (char**) * ('z' - 'a' + 1 + 1)); // dictionary as an array of pointers
                                                                            // to strings representing char codes

    for (int i = 0; i < 'z' - 'a' + 1; i++) {
        *(dictionary + i) = NULL;
    }
    // Decode bit values for first n groups
    for (unsigned long long index = 0; index < totalUniqueCharacters; index++) {
        group* currentGroup = groups + index;
        char characterToBeEncoded = *currentGroup->value;

        unsigned int dictionaryIndex = 0;
        if (characterToBeEncoded == '\000') dictionaryIndex = 'z' - 'a' + 1; // last index of dictionary
        else dictionaryIndex = (int)(characterToBeEncoded - 'a');

        *(dictionary + dictionaryIndex) = calloc(sizeof(char), 10); // TODO: better way of estimating how many bits can be

        // This will get byte value in reversed order
        while (currentGroup->parent != NULL) {
            strcat(*(dictionary + dictionaryIndex), currentGroup->isRightChild ? "1" : "0");
            currentGroup = currentGroup->parent;
        }
        stringReverse(*(dictionary + dictionaryIndex));
        //printf("Character: %c byteValue: %s\n", characterToBeEncoded, *(dictionary + dictionaryIndex));
    }

    for (unsigned long long index = 0; index < totalPossibleGroupsInStore; index++) {
        free((groups + index)->value);
    }

    free(groups);


    FILE* file = fopen("./../input", "r");
    unsigned long size = calculateSizeOfEncodedInput(dictionary, file);
    //printf("Size: %lu\n", size);

    char* binaryString = calloc(sizeof(char), size);

    size_t n = 0;
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    int c;
    while ((c = fgetc(file)) != EOF) {
        unsigned int dictionaryIndex = 0;
        dictionaryIndex = c - 'a';
        //printf("Character: %c byteValue: %s\n", c, *(dictionary + dictionaryIndex));
        strcat(binaryString, *(dictionary + dictionaryIndex));
    }
    strcat(binaryString, *(dictionary + 'z' - 'a' + 1));



    FILE* output = fopen("./../data.bin", "wb");
    if (file == NULL) return 1;
    for (int i = 0; i < strlen(binaryString); i+=8) {
        char part[9];
        strncpy(part, binaryString + i, 8);
        part[8] = '\0';

        unsigned char number = strtol(part, NULL, 2);

        printf("%s: %d\n", part, number);
        fputc(number, output);
    }
    fclose(output);

    output = fopen("./../data.bin", "rb");
    if (output == NULL) return 1;

    // Trzeba zapisać w nagłówku ile będzie liczb po 8 bitów = rozmiar pliku
    for (int i = 0; i < 30; i++) {
        unsigned char number;
        fread(&number, sizeof(unsigned char), 1, output);

        for (int bit = 8; bit; --bit) {  // count from 8 to 1
            putchar(number & (1 << (bit - 1)) ? '1' : '0');
        }

        printf(": %d\n", number);


    }
    fclose(output);

    for (int i = 0; i < 'z' - 'a' + 1; i++) {
        if (*(dictionary + i) != NULL) free(*(dictionary + i));
    }

    free(dictionary);
    free(binaryString);
    fclose(file);
}
