#ifndef STRUTILS_H
#define STRUTILS_H

#include <stdint.h>
#include <stdlib.h>

int detect_endianness(const char *text, size_t *size);
size_t ID3utf16_to_utf8(uint16_t *utf16, size_t utf16len, char **utf8, int isbigend);
size_t ID3iso8859_1_to_utf8(const char *input, char *output);
void ID3proccessFrame(const char *frameData, size_t frameSize, char **output);

#endif
