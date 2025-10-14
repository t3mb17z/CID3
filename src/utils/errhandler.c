#include <utils/errhandler.h>
#include <string.h>

char ERRMESSAGE[ERR_MSG_SIZE];

char *CID3_GetError() {
	return ERRMESSAGE;
}

void CID3_SetError(const char *msg) {
	strncpy(ERRMESSAGE, msg, ERR_MSG_SIZE);
}
