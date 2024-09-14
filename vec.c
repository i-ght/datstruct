#include <string.h>
#include <assert.h>

#include "vec.h"

static size_t calc_unit_addr(
    const struct Vec* vec,
    const size_t index) 
{
    const size_t i = (index + vec->head) * vec->unit_size;
    return i;

}

struct Vec Vec_construct(const size_t data_unit_size)
{
    const struct Vec vec = {
        .unit_count = 0,
        .unit_size = data_unit_size,
        .units = Bytes_construct()
    };
    return vec;
}

void Vec_destruct(struct Vec* vec)
{
    Bytes_destruct(&vec->units);
    vec->unit_count = 0;
    vec->unit_size = 0;
}

static bool value_is_equal(
    const void* unit_value,
    const size_t unit_size,
    const void* user_data)
{
    if (0 == 
        memcmp(
            unit_value,
            user_data,
            unit_size
        )
    ) {
        return true;
    }
    return false;
}

void* Vec_get(
    const struct Vec* vec,
    const size_t i)
{
    const size_t index = calc_unit_addr(vec, i);
    void* address = (void*)&vec->units.data.bytes[index];
    return address;
}

bool Vec_try_find(
    const struct Vec* vec,
    const VecPredicate predicate,
    const void* value_in,
    const void** value)
{
    for (size_t i = 0; i < vec->unit_count; i++) {
        void* val = Vec_get(vec, i);
        if (predicate(
                val,
                vec->unit_size,
                value_in
            )
        ) {
            if (NULL != value) {
                *value = val;
            }
            return true;
        }
    }

    return false;
}

bool Vec_contains(
    const struct Vec* vec,
    const void* value)
{
    return Vec_try_find(
        vec,
        value_is_equal,
        value,
        NULL
    );
}

Code Vec_push_back(
    struct Vec* vec,
    const void* value)
{
    if (ERR == Bytes_write(&vec->units, value, vec->unit_size)) {
        return ERR;
    }

    vec->unit_count++;
    return OK;
}

Code Vec_pop_front(
    struct Vec* vec,
    void* value)
{
    if (vec->unit_count == 0) {
        return ERR;
    }

    const void* head = Vec_get(vec, 0);

    if (value != NULL) {
        const void* _ = memmove(value, head, vec->unit_size);
    }

    vec->head++;
    vec->unit_count--;

    return OK;
}