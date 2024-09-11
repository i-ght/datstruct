#ifndef _MEM_STREAM_
#define _MEM_STREAM_
#include "datstruct.h"

struct MemoryStream
{
    struct Slice slice;
    size_t writ;
};

struct MemoryStream MemoryStream_construct(void);
void MemoryStream_destruct(struct MemoryStream* stream);
Code MemoryStream_write(
    struct MemoryStream* stream,
    const uint8_t* data,
    const size_t len
);
Code MemoryStream_write_cstr(
    struct MemoryStream* stream,
    const char c_str[]
);
size_t MemoryStream_writ(const struct MemoryStream* stream);
Code MemoryStream_set_write_pos(
    struct MemoryStream* stream,
    const size_t write_pos
);
Code MemoryStream_copy_to_cstr(
    const struct MemoryStream* stream,
    char** output
);
#endif