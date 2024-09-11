#ifndef _DAT_STRUCT_
#define _DAT_STRUCT_

#include <stdlib.h>
#include <stdint.h>

typedef enum {ERR=-1, OK=0} Code;
enum {MAX_BUFFER_SIZE=1134903170}; /*gigabyte*/

struct Slice
{
    size_t len;
    uint8_t* data;
};

#endif