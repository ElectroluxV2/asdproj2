#include "shared.h"
#include "heap.h"
#include "queue.h"

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
    // n
    unsigned long long totalUniqueCharacters = 6;
    // There always will be 2n - 1 output groups
    unsigned long long totalPossibleGroupsInStore = totalUniqueCharacters + totalUniqueCharacters - 1;

    group* groups = malloc(sizeof(group) * totalPossibleGroupsInStore);

    // First n groups will be created during counting phase
    unsigned long long  lastFreeGroupsIndex = 0;
    addGroupTBD(groups, lastFreeGroupsIndex++, createDynamicStringTBD("a"), 45);
    addGroupTBD(groups, lastFreeGroupsIndex++, createDynamicStringTBD("b"), 13);
    addGroupTBD(groups, lastFreeGroupsIndex++, createDynamicStringTBD("c"), 12);
    addGroupTBD(groups, lastFreeGroupsIndex++, createDynamicStringTBD("d"), 16);
    addGroupTBD(groups, lastFreeGroupsIndex++, createDynamicStringTBD("e"), 9);
    addGroupTBD(groups, lastFreeGroupsIndex++, createDynamicStringTBD("f"), 5);

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

        printf("left: %s:%lld, right: %s:%lld\n", (char*) left->value, left->count, (char*) right->value, right->count);

        group* pointerToLatestFreeGroup = (groups + lastFreeGroupsIndex++);
        pointerToLatestFreeGroup->value = combineStrings(left->value, right->value);
        pointerToLatestFreeGroup->count = left->count + right->count;

        insert(queue, &currentQueueLength, pointerToLatestFreeGroup);
    }

    free(queue);

    for (unsigned long long index = 0; index < totalPossibleGroupsInStore; index++) {
        free((groups + index)->value);
    }

    free(groups);
}
