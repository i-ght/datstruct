#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "bytes.h"

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

struct Bytes Bytes_construct(void)
{
    struct Bytes bytes = {
        .writ = 0,
        .data = {
            .bytes = NULL,
            .len = 0
        }
    };
    
    return bytes;
}

static Code Bytes_realloc_buffer(
    struct Bytes* bytes,
    const size_t required_space)
{
    if (ERR == realloc_buffer(
        required_space,
        &bytes->data.bytes
    )) {
        return ERR;
    }

    bytes->data.len = required_space;

    return OK;
}

void Bytes_destruct(
    struct Bytes* bytes)
{
    if (NULL != bytes->data.bytes) {
        free(bytes->data.bytes);
    }

    bytes->writ = 0;
    bytes->data.len = 0;
    bytes->data.bytes = NULL;
}

Code Bytes_write(
    struct Bytes* bytes,
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

    enum { DEFAULT_BUFFER_SIZE = 144 };
    #define PHI 1.618033989

    size_t remaining_space = bytes->data.len - bytes->writ;
    while (remaining_space < len) {
        size_t new_size = (bytes->data.len < DEFAULT_BUFFER_SIZE ? DEFAULT_BUFFER_SIZE : bytes->data.len * PHI);

        if (new_size < len) {
            new_size = bytes->writ + len;
        }
        
        
        if (ERR == 
            Bytes_realloc_buffer(
                bytes, 
                new_size
            )
        ) {
            return ERR;
        }

        remaining_space = bytes->data.len - bytes->writ;
    }

    const size_t index = bytes->writ;
    void* target = &bytes->data.bytes[index];
    const void* _ =
        memmove(
            target,
            data,
            len
        );
    bytes->writ += len;

    return OK;
}

Code Bytes_write_cstr(
    struct Bytes* bytes,
    const char c_str[])
{
    return Bytes_write(
        bytes,
        (uint8_t*)c_str,
        strlen(c_str)
    );
}

size_t Bytes_writ(
    const struct Bytes* bytes)
{
    return bytes->writ;
}

Code Bytes_set_write_pos(
    struct Bytes* bytes,
    const size_t write_pos)
{
    if (write_pos >= bytes->data.len) {
        return ERR;
    }

    bytes->writ = write_pos;
    return OK;
}

void Bytes_copy_to_cstr(
    const struct Bytes* bytes,
    char* c_str)
{
    const void* _ =
        memmove(
            c_str,
            bytes->data.bytes,
            bytes->writ
        );
}

