#include <string.h>
#include <stdlib.h>
#include "slice.h"

size_t Slice_len(const struct Slice* slice)
{
    return slice->len;
}

uint8_t* Slice_data(const struct Slice* slice)
{
    return slice->bytes;
}

struct Slice Slice_copy(const struct Slice* slice)
{
    const struct Slice copy = {
        .bytes = calloc(1, slice->len),
        .len = 0
    };
    if (copy.bytes == NULL) {
        abort();
    }
    const void* _ =
        memmove(
            copy.bytes,
            slice->bytes,
            slice->len
        );
    return copy;
}