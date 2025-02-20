#ifndef STRUTILS_H
#define STRUTILS_H

#include <stdint.h>
#include <stdlib.h>

int detect_endianness(uint16_t *text, size_t *size);
size_t ID3utf16_to_utf8(uint16_t *utf16, size_t utf16len, char *utf8, int isbigend);
size_t ID3iso8859_1_to_utf8(const char *input, char *output);

#endif
