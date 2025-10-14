#include <stdlib.h>
#include <utils/errhandler.h>

void *CID3_Alloc(size_t size, size_t nmem, const char *mess) {
	void *temp = calloc(size, 1);
	if(temp == NULL) {
		CID3_SetError(mess);
		return NULL;
	}
	return temp;
}
