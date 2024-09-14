#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "vec.h"

static void Bytes_test_hello_world()
{
    struct Bytes bytes = Bytes_construct();
    Code code = ERR;
    
    static const char hello[] = "hello ";
    static const char world[] = "world";
    static const char hello_world[] = "hello world";

    code = Bytes_write_cstr(&bytes, hello);
    assert(OK == code);

    assert(Bytes_writ(&bytes) == strlen(hello));

    code = Bytes_write_cstr(&bytes, world);
    assert(Bytes_writ(&bytes) == strlen(hello) + strlen(world));
    assert(OK == code);

    char copy[8192] = {0};
    Bytes_copy_to_cstr(&bytes, copy);

    assert(0 == strcmp(hello_world, copy));
    
    static const char long_string[] = "helllllooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo world.";
    Bytes_write_cstr(&bytes, long_string);

    Bytes_destruct(&bytes);
}

static bool value_is_200(
    const void* value,
    const size_t _data_length,
    const void* _user_data)
{
    const int* i = (int*)value;
    return *i == 200;
}

static bool value_is_301(
    const void* value,
    const size_t _data_length,
    const void* _user_data)
{
    const int* i = (int*)value;
    return *i == 301;
}

static void Vec_test_200_ints()
{
    struct Vec v = Vec_construct(sizeof(int));

    int value = 200;

    Code code = ERR;

    for (int i = 0; i < 200; i++) {
        code = Vec_push_back(&v, &value);
        assert(OK == code);
    }

    for (int i = 0; i < 200; i++) {
        const int* retrieved_value = Vec_get(&v, i);
        assert(*retrieved_value == 200);
    }

    const int* two_hundo = NULL;
    assert(Vec_try_find(&v, value_is_200, NULL, (const void**)&two_hundo));

    value = 301;
    code = Vec_push_back(&v, &value);
    assert(code == OK);

    const int* three_hundo_one = NULL;
    assert(Vec_try_find(&v, value_is_301, NULL, (const void**)&three_hundo_one));

    three_hundo_one = Vec_get(&v, 200);
    assert(*three_hundo_one == 301);

    int popped = -1;
    assert(Vec_pop_front(&v, &popped) == OK);

    Vec_destruct(&v);
}

int main(void)
{
    Bytes_test_hello_world();
    Vec_test_200_ints();
    return 0;
}
