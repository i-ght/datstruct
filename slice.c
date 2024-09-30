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
