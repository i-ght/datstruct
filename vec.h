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
    const void* user_data,
    const void** value
);
bool Vec_contains(const struct Vec* vec, const void* value);
Code Vec_push(struct Vec* vec, const void* value);
void* Vec_get(const struct Vec* vec, const size_t i);

#endif