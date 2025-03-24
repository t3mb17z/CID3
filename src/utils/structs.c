#include <stdlib.h>
#include <string.h>
#include <utils/structs.h>

void deepCopy(CID3Tag *dest, CID3Tag *src) {
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
