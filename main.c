#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct group {
    char* value;
    unsigned long long count;
};

struct array {
    struct group* pointer;
    unsigned long long length;
};

const long long RAW_INPUT_BUFFER_NUMBER = 50;
struct array getArrayFromStdIn();
void printArray(struct array array);
void swap(struct group* first, struct group* second);
void heapify(struct array* array, unsigned long long index);
void buildHeap(struct array* array);
struct group extract(struct array* queue);
void insert(struct array* queue, struct group item);

int main() {
    struct array array;
    array.pointer = malloc(sizeof(struct group) * 6);
    array.length = 6;

    struct group g0;
    g0.value = malloc(sizeof(char));
    *g0.value = 'a';
    g0.count = 45;
    *array.pointer = g0;

    struct group g1;
    g1.value = malloc(sizeof(char));
    *g1.value = 'b';
    g1.count = 13;
    *(array.pointer + 1) = g1;

    struct group g2;
    g2.value = malloc(sizeof(char));
    *g2.value = 'c';
    g2.count = 12;
    *(array.pointer + 2) = g2;

    struct group g3;
    g3.value = malloc(sizeof(char));
    *g3.value = 'd';
    g3.count = 16;
    *(array.pointer + 3) = g3;

    struct group g4;
    g4.value = malloc(sizeof(char));
    *g4.value = 'e';
    g4.count = 9;
    *(array.pointer + 4) = g4;

    struct group g5;
    g5.value = malloc(sizeof(char));
    *g5.value = 'f';
    g5.count = 5;
    *(array.pointer + 5) = g5;

    buildHeap(&array);

    unsigned long long originalLength = array.length;

    for (unsigned long long i = 0; i < originalLength - 1; i++) {
        struct group left = extract(&array);
        struct group right = extract(&array);

        printf("left: %lld, right: %lld\n", left.count, right.count);
        struct group toInsert;
        toInsert.value = malloc(sizeof(left.value) + sizeof(right.value));
        toInsert.count = left.count + right.count;
        strcpy(toInsert.value, left.value);
        strcat(toInsert.value, right.value);

        free(left.value);
        free(right.value);

        insert(&array, toInsert);
    }

    printArray(array);

    for (unsigned long long i = 0; i < array.length; i++) {
        struct group group = *(array.pointer + i);
        free(group.value);
    }

    free(array.pointer);
    return 0;
}

void heapify(struct array* array, const unsigned long long index) {
    unsigned long long largestIndex = index;
    unsigned long long leftIndex = (2 * index) + 1;
    unsigned long long rightIndex = (2 * index) + 2;

    if (leftIndex < array->length && (array->pointer + leftIndex)->count < (array->pointer + largestIndex)->count) {
        largestIndex = leftIndex;
    }

    if (rightIndex < array->length && (array->pointer + rightIndex)->count < (array->pointer + largestIndex)->count) {
        largestIndex = rightIndex;
    }

    if (largestIndex != index) {
        swap(array->pointer + index, array->pointer + largestIndex);
        heapify(array, largestIndex);
    }
}

void buildHeap(struct array* array) {
    for (unsigned long long i = (array->length / 2) - 1; i != (unsigned long long) (-1); --i) {
        heapify(array, i);
    }
}

struct group extract(struct array* queue) {
    struct group item = *(queue->pointer);
    *(queue->pointer) = *(queue->pointer + queue->length - 1);
    queue->length = queue->length - 1;
    buildHeap(queue);
    return item;
}

void insert(struct array* queue, struct group item) {
    // WARNING this is totally unsafe, and you must manually control not leading to overflow.
    // Maximum items in queue is constant and assigned during array creation.
    *(queue->pointer + queue->length) = item;
    queue->length = queue->length + 1;
    buildHeap(queue);
}

void printArray(const struct array array) {
    printf("Array (%lld) [", array.length);
    for (long long i = 0; i < array.length; i++) {
        struct group group = *(array.pointer + i);
        printf("%s:%lld%s", group.value, group.count, i + 1 == array.length ? "" : ", ");
    }
    printf("]\n");
}

void swap(struct group* first, struct group* second) {
    const struct group tmp = *first;
    *first = *second;
    *second = tmp;
}
