#include "queue.h"

static void incrOrLoopBack(
    size_t* i,
    const size_t max)
{
    size_t tmp = *i + 1;
    if (tmp >= max)
        tmp = 0;
    *i = tmp;
}

struct Queue Queue_construct(
    const size_t capacity,
    const void* values[])
{
    const struct Queue q = {
        .head = 0,
        .tail = 0,
        .capacity = capacity,
        .count = 0,
        .values = values
    };
    return q;
}

bool ref_q_full(
    const struct Queue* queue)
{
    return queue->count == queue->capacity;
}


Code ref_q_enqueue(
    struct Queue* queue,
    void* value)
{
    if (ref_q_full(queue))
        return ERR;
    
    const size_t i = queue->tail;
    queue->values[i] = value;
    incrOrLoopBack(
        &queue->tail,
        queue->capacity
    );
    queue->count++;
    return OK;
}