//
// Created by mate on 17.12.2021.
//

#ifndef ASDPROJ2_OCCURRENCESCOUNT_H
#define ASDPROJ2_OCCURRENCESCOUNT_H

#include "shared.h"

unsigned short countTotalNonZeroOccurrences(const unsigned long long* occurrences) {
    unsigned short count = 0;
    for (unsigned short i = 0; i < TOTAL_POSSIBLE_ASCII_KEYS; i++) {
        if ((*(occurrences + i)) == 0) continue;
        count++;
    }
    return count;
}

unsigned long long* countOccurrencesOfCharactersInFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) return NULL; // Could not open file

    unsigned long long* occurrences = calloc(sizeof(unsigned long long),TOTAL_POSSIBLE_ASCII_KEYS);
    char c;
    while ((c = (char) fgetc(file)) != EOF) {
        (*(occurrences + c))++;
    }

    fclose(file);
    return occurrences;
}


group* makeGroupsFromOccurrences(const unsigned long long int *occurrences, unsigned long long int totalPossibleGroupsInStore, unsigned long long *lastFreeGroupsIndex) {
    group* groups = calloc(sizeof(group), totalPossibleGroupsInStore);

    for (char c = 0; c < TOTAL_POSSIBLE_ASCII_KEYS; c++) {
        if ((*(occurrences + c)) == 0) continue; // There is no other way of skipping empty values

        group* currentGroup = (groups + (*lastFreeGroupsIndex)++);
        currentGroup->value = convertCharacterToString(c);
        currentGroup->count = *(occurrences + c);
    }

    // End of context char '\0'
    group* trailingNull = (groups + (*lastFreeGroupsIndex)++);
    trailingNull->value = convertCharacterToString('\0');
    trailingNull->count = 1;

    return groups;
}

#endif //ASDPROJ2_OCCURRENCESCOUNT_H
