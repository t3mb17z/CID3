#ifndef STRUTILS_H
#define STRUTILS_H

#include <stdint.h>
#include <stdlib.h>

typedef enum {
	BIG_END,
	LITTLE_END,
} ENDS;

uint16_t *endianness(uint16_t *text, size_t *size, ENDS *endianness);
size_t ID3utf16_to_ut1f8(uint16_t *utf16, size_t utf16len, char *utf8);

#endif
