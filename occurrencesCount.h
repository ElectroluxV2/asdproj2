//
// Created by mate on 17.12.2021.
//

#ifndef ASDPROJ2_OCCURRENCESCOUNT_H
#define ASDPROJ2_OCCURRENCESCOUNT_H

#include "shared.h"

// Total supported ascii keys
static const unsigned TOTAL_POSSIBLE_KEYS_IN_OCCURRENCES_MAP = '~' - ' ' + 1;
// Amount we need to subtract from evey key to start indexing from 0 ak. int value of the lowest possible char key.
static const unsigned KEY_OFFSET_IN_OCCURRENCES_MAP = ' ';

unsigned long long* makeOccurrencesMap() {
    return calloc(sizeof(unsigned long long),TOTAL_POSSIBLE_KEYS_IN_OCCURRENCES_MAP);
}

unsigned long long* getOccurrences(unsigned long long* occurrences, const char key) {
    return occurrences + (key - KEY_OFFSET_IN_OCCURRENCES_MAP);
}

unsigned short countTotalNonZeroOccurrences(const unsigned long long* occurrences) {
    unsigned short count = 0;
    for (unsigned short i = 0; i < TOTAL_POSSIBLE_KEYS_IN_OCCURRENCES_MAP; i++) {
        if ((*(occurrences + i)) == 0) continue;
        count++;
    }
    return count;
}

unsigned long long* countOccurrencesOfCharactersInFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) return NULL; // Could not open file

    unsigned long long* occurrences = makeOccurrencesMap();
    char c;
    while ((c = (char) fgetc(file)) != EOF) {
        (*getOccurrences(occurrences, c))++;
    }

    fclose(file);
    return occurrences;
}

char convertIntegerToCharacterKey(int integer) {
    return (char) (integer + KEY_OFFSET_IN_OCCURRENCES_MAP);
}

char* convertIntToCharacterSingletonArray(char character) {
    char* dynamic = malloc(sizeof(char) * 2);
    *dynamic = character;
    *(dynamic + 1) = '\0';
    return dynamic;
}

group* makeGroupsFromOccurrences(const unsigned long long int *occurrences, unsigned long long int totalPossibleGroupsInStore, unsigned long long *lastFreeGroupsIndex) {
    group* groups = calloc(sizeof(group), totalPossibleGroupsInStore);

    for (unsigned short i = 0; i < TOTAL_POSSIBLE_KEYS_IN_OCCURRENCES_MAP; i++) {
        if ((*(occurrences + i)) == 0) continue; // There is no other way of skipping empty values

        group* currentGroup = (groups + (*lastFreeGroupsIndex)++);
        currentGroup->value = convertIntToCharacterSingletonArray(convertIntegerToCharacterKey(i));
        currentGroup->count = *(occurrences + i);
    }

    // End of context char '\0'
    group* trailingNull = (groups + (*lastFreeGroupsIndex)++);
    trailingNull->value = convertIntToCharacterSingletonArray('\0');
    trailingNull->count = 1;

    return groups;
}

#endif //ASDPROJ2_OCCURRENCESCOUNT_H
