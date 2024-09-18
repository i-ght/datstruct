#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"
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
    assert(OK == Bytes_write_cstr(&bytes, long_string));

    Bytes_destruct(&bytes);
}

static bool value_is_200(
    const void* value,
    const size_t _unit_size,
    const void* _user_data)
{
    const int* i = (int*)value;
    return *i == 200;
}

static bool value_is_301(
    const void* value,
    const size_t _unit_size,
    const void* _user_data)
{
    const int* i = (int*)value;
    return *i == 301;
}

static void Vec_test_dequeue_99999_ints()
{
    struct Vec v = Vec_construct(sizeof(int));

    int value = 999999;
    for (size_t i = 0; i < 99999; i++) {
        assert(OK == Vec_push_back(&v, &value));
    }

    while (v.unit_count > 0) {
        int unit = 0;
        assert(OK == Vec_pop_front(&v, &unit));
        assert(unit == value);
    }

    Vec_destruct(&v);
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

    size_t index = 0;
    const int* two_hundo = NULL;
    assert(Vec_try_find(&v, value_is_200, NULL, &index, (const void**)&two_hundo));
    assert(index == 0);
    assert(*two_hundo == 200);

    value = 301;
    code = Vec_push_back(&v, &value);
    assert(code == OK);

    const int* three_hundo_one = NULL;
    assert(Vec_try_find(&v, value_is_301, NULL, &index, (const void**)&three_hundo_one));
    assert(index == 200);

    three_hundo_one = Vec_get(&v, 200);
    assert(*three_hundo_one == 301);

    int popped = -1;
    assert(Vec_pop_front(&v, &popped) == OK);

    Vec_destruct(&v);
}

static void Hash_test()
{
    Hash hash = {0};

    char* value = "hello world";

    char s[200][4] = {0};

    for (int i = 0; i < 200; i++) {
        const int check = snprintf(s[i], 4, "%d", i);
        assert(check >= 0 && check <= 3);
        void* ex_value = Hash_insert(&hash, s[i], s[i]);
        assert(NULL == ex_value);
    }


    const char* key = "200";
    assert(NULL == Hash_insert(&hash, key, value));
    assert(value == Hash_find(&hash, key));
    assert(value == Hash_insert(&hash, key, value));
    
    assert(value == Hash_insert(&hash, key, NULL));
    assert(NULL == Hash_insert(&hash, key, NULL));

    HashElem *p = NULL;
    for (p = Hash_first(&hash); p; p = Hash_next(p)){
        char* data = Hash_data(p);
        printf("%s\n", data);
        /* free(data); */
    }
 

    Hash_clear(&hash);
}

int main(void)
{
    Bytes_test_hello_world();
    Vec_test_dequeue_99999_ints();
    Vec_test_200_ints();
    Hash_test();
    return 0;
}
