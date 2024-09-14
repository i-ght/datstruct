#ifndef _VEC_H_
#define _VEC_H_

#include <stdbool.h>
#include <stdlib.h>
#include "bytes.h"

struct Vec
{
    size_t unit_count;
    size_t unit_size;
    struct Bytes units;
    size_t head;
};

typedef bool (*VecPredicate)(
    const void* value,
    const size_t value_length,
    const void* user_data
);

struct Vec Vec_construct(const size_t data_unit_size);
void Vec_destruct(struct Vec* vec);

bool Vec_try_find(
    const struct Vec* vec,
    const VecPredicate predicate,
    const void* value_in,
    const void** value
);
bool Vec_contains(const struct Vec* vec, const void* value);
Code Vec_push_back(struct Vec* vec, const void* value);
void* Vec_get(const struct Vec* vec, const size_t i);
Code Vec_pop_front(struct Vec* vec, void* value);
#endif