//
// Created by mate on 16.12.2021.
//

#ifndef ASDPROJ2_QUEUE_H
#define ASDPROJ2_QUEUE_H

#include "shared.h"

struct group* extract(struct group** queue, unsigned long long* currentQueueLength) {
    group* item = *(queue);
    *(queue) = *(queue + *currentQueueLength - 1);
    *currentQueueLength = *currentQueueLength - 1;
    buildHeap(queue, *currentQueueLength);
    return item;
}

// WARNING this is totally unsafe, and you must manually control to not lead to overflow.
// Maximum items in queue is constant and assigned during array creation.
void insert(struct group** queue, unsigned long long* currentQueueLength, struct group* item) {
    *(queue + *currentQueueLength) = item;
    *currentQueueLength = *currentQueueLength + 1;
    buildHeap(queue, *currentQueueLength);
}

#endif //ASDPROJ2_QUEUE_H
