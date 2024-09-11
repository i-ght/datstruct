#ifndef _SLICE_H_
#define _SLICE_H_

#include <stdlib.h>
#include <stdint.h>

#include "datstruct.h"

struct Slice
{
    size_t len;
    uint8_t* bytes;
};

size_t Slice_len(const struct Slice* slice);
uint8_t* Slice_data(const struct Slice* slice);

#endif