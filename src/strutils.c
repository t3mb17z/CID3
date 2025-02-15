#include <stdint.h>
#include <stdio.h>
#include <string.h>

int detect_endianness(uint16_t *text, size_t *size) {
	if (text[0] == 0xFEFF) {
		(*size)--;
		return 1; // Big-endian
	} else if (text[0] == 0xFFFE) {
		(*size)--;
		return 0; // Little-endian
	}
	return -1; // No BOM presente
}

size_t ID3utf16_to_utf8(uint16_t *utf16, size_t utf16len, char *utf8, int is_big_endian) {
	size_t i = 0, j = 0;
	
	while (i < utf16len) {
		uint16_t code_unit = utf16[i++];
		
		/*if (is_big_endian) {
			code_unit = (code_unit >> 8) | (code_unit << 8); // Convertir endianess si es necesario
		}*/
		
		if (code_unit >= 0xD800 && code_unit <= 0xDBFF) { // Es un par sustituto (surrogate pair)
			if (i < utf16len) {
				uint16_t low = utf16[i++];
				if (is_big_endian) {
					low = (low >> 8) | (low << 8);
				}
				if (low >= 0xDC00 && low <= 0xDFFF) {
					uint32_t code_point = ((code_unit - 0xD800) << 10) + (low - 0xDC00) + 0x10000;
					utf8[j++] = (char)(0xF0 | (code_point >> 18));
					utf8[j++] = (char)(0x80 | ((code_point >> 12) & 0x3F));
					utf8[j++] = (char)(0x80 | ((code_point >> 6) & 0x3F));
					utf8[j++] = (char)(0x80 | (code_point & 0x3F));
				}
			}
		} else if (code_unit <= 0x7F) { // ASCII
			utf8[j++] = (char)code_unit;
		} else if (code_unit <= 0x7FF) { // 2 bytes
			utf8[j++] = (char)(0xC0 | (code_unit >> 6));
			utf8[j++] = (char)(0x80 | (code_unit & 0x3F));
		} /* else if(code_unit <= 0xFFFF) { // 3 bytes
			utf8[j++] = (char)(0xE0 | (code_unit >> 12));
			utf8[j++] = (char)(0xE0 | ((code_unit >> 6) & 0x3F));
			utf8[j++] = (char)(0x80 | (code_unit & 0x3F));
		}*/
	}
	
	utf8[j] = '\0';
	return j;
}
