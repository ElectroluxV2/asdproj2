//
// Created by mate on 16.12.2021.
//

#ifndef ASDPROJ2_HEAP_H
#define ASDPROJ2_HEAP_H

#include "shared.h"

void heapify(group** array, const unsigned long long arrayLength, const unsigned long long index) {
    unsigned long long largestIndex = index;
    unsigned long long leftIndex = (2 * index) + 1;
    unsigned long long rightIndex = (2 * index) + 2;

    if (leftIndex < arrayLength && (*(array + leftIndex))->count < (*(array + largestIndex))->count) {
        largestIndex = leftIndex;
    }

    if (rightIndex < arrayLength && (*(array + rightIndex))->count < (*(array + largestIndex))->count) {
        largestIndex = rightIndex;
    }

    if (largestIndex != index) {
        swap(array + index, array + largestIndex);
        heapify(array, arrayLength, largestIndex);
    }
}

void buildHeap(group** array, const unsigned long long arrayLength) {
    for (unsigned long long i = (arrayLength / 2) - 1; i != (unsigned long long) -1; --i) {
        heapify(array, arrayLength, i);
    }
}

#endif //ASDPROJ2_HEAP_H
