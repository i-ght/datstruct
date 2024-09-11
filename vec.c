#include <string.h>
#include <assert.h>

#include "vec.h"


struct Vec Vec_construct(const size_t data_unit_size)
{
    const struct Vec vec = {
        .unit_count = 0,
        .unit_size = data_unit_size,
        .units = MemoryStream_construct()
    };
    return vec;
}

void Vec_destruct(struct Vec* vec)
{
    MemoryStream_destruct(&vec->units);
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

bool Vec_try_find(
    const struct Vec* vec,
    const VecPredicate predicate,
    const void* user_data,
    const void** value)
{
    for (size_t i = 0; i < vec->unit_count; i++) {
        const size_t index = i * vec->unit_size;
        void* val = &vec->units.slice.data[index];
        if (predicate(
                val,
                vec->unit_size,
                user_data
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

bool Vec_contains(const struct Vec* vec, const void* value)
{
    return Vec_try_find(
        vec,
        value_is_equal,
        NULL,
        &value
    );
}

Code Vec_push(struct Vec* vec, const void* value)
{
    if (ERR == MemoryStream_write(&vec->units, value, vec->unit_size)) {
        return ERR;
    }

    vec->unit_count++;
    return OK;
}

void* Vec_get(const struct Vec* vec, const size_t i)
{
    const size_t address_of_unit = i * vec->unit_size;
    void* address = (void*)&vec->units.slice.data[address_of_unit];
    return address;
}

static void test_200_ints()
{
    struct Vec v = Vec_construct(sizeof(int));

    const int value = 200;

    Code code = {0};

    for (int i = 0; i < 200; i++) {
        code = Vec_push(&v, &value);
        assert(OK == code);
    }

    for (int i = 0; i < 200; i++) {
        const int* retrieved_value = Vec_get(&v, i);
        assert(*retrieved_value == 200);
    }

    Vec_destruct(&v);
}

static void test()
{
    test_200_ints();
}

void main(void)
{
    test();
}