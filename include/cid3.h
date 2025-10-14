#ifndef CID3_H
#define CID3_H

#define HEADER_SIZE 10
#define FRAME_CONTENT 4096

typedef struct {
	char id[4];
	int major;
	int revision;
	char flags[4];
	int size;
} CID3_Header;

typedef struct {
	char id[5];
	int size;
	char *text;
	int encoding;
} CID3_Tag;

typedef struct {
	CID3_Tag *tags;
	int count;
} CID3_TagCollection;

typedef struct {
	char *name;
	char *parent;
	char *full_path;
	CID3_Header header;
	CID3_TagCollection tags;
} CID3_FileRef;

int CID3_ReadTags(CID3_FileRef **fileref, CID3_TagCollection *tag);
int CID3_ReadHeader(CID3_FileRef **fileref, CID3_Header *header);
int CID3_ReadFile(const char *path, CID3_FileRef **fileref);

#endif
