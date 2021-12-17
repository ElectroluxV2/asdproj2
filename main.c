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

int main() {
    unsigned long long* occurrences = countOccurrencesOfCharactersInFile("./../input");
    // n
    unsigned long long totalUniqueCharacters = countNonZero(occurrences);
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

//        printf("left: %s:%lld, right: %s:%lld\n", (char*) left->value, left->count, (char*) right->value, right->count);

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

    // Decode bit values for first n groups
    for (unsigned long long index = 0; index < totalUniqueCharacters; index++) {
        group* currentGroup = groups + index;
        char characterToBeEncoded = *currentGroup->value;
        char* byteValue = calloc(sizeof(char), 10); // TODO: better way of estimating how many bits can be

        // This will get byte value in reversed order
        while (currentGroup->parent != NULL) {
            strcat(byteValue, currentGroup->isRightChild ? "1" : "0");

            currentGroup = currentGroup->parent;
        }

        printf("Character: %c byteValue: %s\n", characterToBeEncoded, stringReverse(byteValue));
        free(byteValue);
    }

    for (unsigned long long index = 0; index < totalPossibleGroupsInStore; index++) {
        free((groups + index)->value);
    }

    free(groups);
}
