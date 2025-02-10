#include <stdint.h>
#include <stdio.h>
#include <strutils.h>

uint16_t *endianness(uint16_t *text, size_t *size, ENDS *end) {
	if(text[0] == 0xFEFF) {
		*end = BIG_END;
		text++;
		(*size)--;
	} else if(text[0] == 0xFFFE) {
		*end = LITTLE_END;
		text++;
		(*size)--;
	}

	return text;
};

size_t ID3utf16_to_ut1f8(uint16_t *utf16, size_t utf16len, char *utf8) {
	size_t i = 0, j = 0;
	while(i < utf16len) {
		uint16_t code_unit = utf16[i++];
		if(code_unit >= 0xD800 && code_unit <= 0xDBFF) {
			uint16_t low = utf16[i++];
			if(low >= 0xDC00 && low <= 0xDFFF) {
				uint32_t code_point = ((code_unit - 0xD800) << 10) + (low - 0xDC00) + 0x10000;
				utf8[j++] = (char)(0xF0 | (code_point >> 18));
				utf8[j++] = (char)(0x80 | ((code_point >> 12) & 0x3F));
				utf8[j++] = (char)(0x80 | ((code_point >> 6) & 0x3F));
				utf8[j++] = (char)(0x80 | (code_point & 0x3F));
			}
		} else if(code_unit <= 0x7F) {
			utf8[j++] = (char)code_unit;
		} else if(code_unit <= 0x7FF) {
			utf8[j++] = (char)(0xC0 | (code_unit >> 6));
			utf8[j++] = (char)(0x80 | (code_unit & 0x3F));
		}
		else {
			utf8[j++] = (char)((0xE0 | (code_unit >> 12)));
			utf8[j++] = (char)((0x80 | (code_unit >> 6) & 0x3F));
			utf8[j++] = (char)(0xE0 | (code_unit & 0x3F));
		}
	}

	utf8[j] = '\0';
	return j;
}
