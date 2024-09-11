#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdlib.h>
#include "vec.h"

struct Queue
{
    size_t head;
    size_t tail;
    size_t count;
    size_t capacity;
    void** values;
};

struct Queue Queue_construct(
    const size_t capacity,
    const void* values[]
);
#endif