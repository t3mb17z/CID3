#include <stdlib.h>
#include <string.h>
#include <utils/structs.h>

void deepCopy(CID3_Tag *dest, CID3_Tag *src) {
	if(src->text) {
		dest->text = calloc(strlen(src->text) + 1, 1);
		if(dest->text)
			strncpy(dest->text, src->text, strlen(src->text));
		dest->size = src->size;
		strncpy(dest->id, src->id, 4);
		dest->encoding = src->encoding;
	} else {
		dest->text = NULL;
	}
}
