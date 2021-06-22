#ifndef ENCODER_HEADER
#define ENCODER_HEADER

#include <stdlib.h>
#include <stdint.h>

typedef struct utf_8
{
    size_t size;
    unsigned char bytes[4];
}utf_8;

extern const utf_8 cp1252[256];
#endif
