//
// Created by mate on 17.12.2021.
//

#ifndef ASDPROJ2_OCCURRENCESCOUNT_H
#define ASDPROJ2_OCCURRENCESCOUNT_H

#include "shared.h"

//static const unsigned long long array[94]; // Total supported ascii keys (' ' - '~')
//static const unsigned long long* get(char key) {
//    return array + (key - ' '); // Space has the lowest
//}

unsigned long long* makeMap() {
    return calloc(sizeof(unsigned long long),94); // Total supported ascii keys (' ' - '~')
}

unsigned long long* getValue(unsigned long long* map, const int key) {
    return map + (key - ' '); // Space has the lowest ascii value
}

unsigned short countNonZero(const unsigned long long* map) {
    unsigned short count = 0;
    for (unsigned short i = 0; i < 94; i++) {
        if ((*(map + i)) > 0) {
            count++;
        }
    }
    return count;
}

unsigned long long* countOccurrencesOfCharactersInFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    size_t n = 0;

    if (file == NULL) return NULL; // Could not open file

    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned long long* map = makeMap();
    int c;
    while ((c = fgetc(file)) != EOF) {
        (*getValue(map, c))++;
    }

    fclose(file);
    return map;
}

char* convertIntToCharacterSingletonArray(int character) {
    const char c = (char) (character + ' ');
    char* dynamic = malloc(sizeof(char) * 2);
    *dynamic = c;
    *(dynamic + 1) = '\0';
    return dynamic;
}

group* getGroupsFromOccurrences(const unsigned long long int *occurrences, unsigned long long int totalPossibleGroupsInStore, unsigned long long *lastFreeGroupsIndex) {
    group* groups = malloc(sizeof(group) * totalPossibleGroupsInStore);

    for (unsigned short i = 0; i < 94; i++) {
        if ((*(occurrences + i)) == 0) continue;

        group* currentGroup = (groups + (*lastFreeGroupsIndex)++);
        currentGroup->value = convertIntToCharacterSingletonArray(i);
        currentGroup->count = *(occurrences + i);
    }

    return groups;
}

#endif //ASDPROJ2_OCCURRENCESCOUNT_H
