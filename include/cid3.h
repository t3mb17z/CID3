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
} CID3Header;

typedef struct {
	char id[5];
	int size;
	char *text;
	int encoding;
} CID3Tag;

typedef struct {
	CID3Tag *tags;
	int count;
} CID3TagCollection;

typedef struct {
	char *name;
	char *parent;
	char *full_path;
	CID3Header header;
	CID3TagCollection tags;
} CID3FileRef;

void CID3readTags(CID3FileRef fileref, CID3TagCollection *tag);
void CID3readHeader(CID3FileRef fileref, CID3Header *header);
void CID3readFile(const char *path, CID3FileRef *fileref);

#endif
