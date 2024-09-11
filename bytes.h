#ifndef _MEM_STREAM_
#define _MEM_STREAM_
#include "slice.h"

struct Bytes
{
    struct Slice data;
    size_t writ;
};

struct Bytes Bytes_construct(void);
void Bytes_destruct(struct Bytes* bytes);
Code Bytes_write(
    struct Bytes* bytes,
    const uint8_t data[],
    const size_t len
);
Code Bytes_write_cstr(
    struct Bytes* bytes,
    const char c_str[]
);
size_t Bytes_writ(const struct Bytes* stream);
Code Bytes_set_write_pos(
    struct Bytes* bytes,
    const size_t write_pos
);
void Bytes_copy_to_cstr(
    const struct Bytes* bytes,
    char* c_str
);
#endif