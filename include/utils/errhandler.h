#ifndef ERRHANDLER_H
#define ERRHANDLER_H

#define ERR_MSG_SIZE 256

char *CID3_GetError(void);
void CID3_SetError(const char *msg);

#endif
