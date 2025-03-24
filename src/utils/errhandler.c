#include <utils/errhandler.h>
#include <string.h>

char ERRMESSAGE[ERR_MSG_SIZE];

char *CID3getError() {
	return ERRMESSAGE;
}

void CID3setError(char *msg) {
	strncpy(ERRMESSAGE, msg, ERR_MSG_SIZE);
}
