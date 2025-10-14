#ifndef STRUTILS_H
#define STRUTILS_H

#include <stdint.h>
#include <stdlib.h>

int detect_endianness(const char *text, size_t *size);
size_t CID3_Utf16ToUtf8(uint16_t *utf16, size_t utf16len, char **utf8, int isbigend);
size_t CID3_ISO8859_1ToUtf8(const char *input, char *output);
void CID3_ProccessFrame(const char *frameData, size_t frameSize, char **output);

#endif
