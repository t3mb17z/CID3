#ifndef ERRHANDLER_H
#define ERRHANDLER_H

#define ERR_MSG_SIZE 256

char *CID3getError(void);
void CID3setError(const char *msg);

#endif
