#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "memstream.h"

static Code realloc_buffer(
    const size_t required_space,
    uint8_t** buffer)
{
    if (required_space > MAX_BUFFER_SIZE) {
        return ERR;
    }

    char* tmp = 
        realloc(
            *buffer,
            required_space
        );
    if (NULL == tmp) {
        return ERR;
    }
    
    *buffer = tmp;
    return OK;

}

struct MemoryStream MemoryStream_construct(void)
{
    struct MemoryStream stream = {
        .writ = 0,
        .slice = {
            .data = NULL,
            .len = 0
        }
    };
    
    return stream;
}

static Code MemoryStream_realloc_buffer(
    struct MemoryStream* stream,
    const size_t required_space)
{
    if (ERR == realloc_buffer(
        required_space,
        &stream->slice.data
    )) {
        return ERR;
    }

    stream->slice.len = required_space;

    return OK;
}

void MemoryStream_destruct(
    struct MemoryStream* stream)
{
    if (NULL != stream->slice.data) {
        free(stream->slice.data);
    }

    stream->writ = 0;
    stream->slice.len = 0;
    stream->slice.data = NULL;
}

Code MemoryStream_write(
    struct MemoryStream* stream,
    const uint8_t* data,
    const size_t len)
{
    if (NULL == data) {
        return ERR;
    }
    if (len == 0) {
        return ERR;
    }
    if (len > MAX_BUFFER_SIZE) {
        return ERR;
    }

    enum {DEFAULT_BUFFER_SIZE=144};
    #define PHI 1.618033989

    size_t remaining_space = stream->slice.len - stream->writ;
    while (remaining_space < len) {
        const size_t new_size = (stream->slice.len < DEFAULT_BUFFER_SIZE ? DEFAULT_BUFFER_SIZE : stream->slice.len * PHI);
        
        if (ERR == MemoryStream_realloc_buffer(
            stream, 
            new_size)
        ) {
            return ERR;
        }

        remaining_space = stream->slice.len - stream->writ;
    }

    const size_t i = stream->writ;
    void* target = &stream->slice.data[i];
    const void* _ =
        memmove(
            target,
            data,
            len
        );
    stream->writ += len;

    return OK;
}

Code MemoryStream_write_cstr(
    struct MemoryStream* stream,
    const char c_str[])
{
    return MemoryStream_write(
        stream,
        (uint8_t*)c_str,
        strlen(c_str)
    );
}

size_t MemoryStream_writ(
    const struct MemoryStream* stream)
{
    return stream->writ;
}

Code MemoryStream_set_write_pos(
    struct MemoryStream* stream,
    const size_t write_pos)
{
    if (write_pos >= stream->slice.len) {
        return ERR;
    }

    stream->writ = write_pos;
    return OK;
}

Code MemoryStream_copy_to_cstr(
    const struct MemoryStream* stream,
    char** output)
{
    char* tmp = calloc(1, stream->writ + 1);
    if (NULL == tmp) {
        return ERR;
    }

    const void* _ =
        memmove(
            tmp,
            stream->slice.data,
            stream->writ
        );

    *output = tmp;

    return OK;
}

static void test_hello_world()
{
    struct MemoryStream stream = MemoryStream_construct();
    Code code = -1;
    
    static const char hello_world[] = "hello world";
    static const char hello_world_long[] = "hello world hello world hello world";
    static const char hello_world_combined[] = "hello worldhello world hello world hello world ";

    code = MemoryStream_write_cstr(&stream, hello_world);
    assert(OK == code);

    assert(MemoryStream_writ(&stream) == strlen(hello_world));

    code = MemoryStream_write_cstr(&stream, hello_world_long);
    assert(MemoryStream_writ(&stream) == strlen(hello_world) + strlen(hello_world_long));
    assert(OK == code);

    char* copy = NULL;
    code = MemoryStream_copy_to_cstr(&stream, &copy);
    assert(0 == memcmp(copy, hello_world_combined, MemoryStream_writ(&stream)));
    
    assert(OK == code);
}

static void test()
{
    test_hello_world();
}
