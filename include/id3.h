#ifndef ID3_H
#define ID3_H

#define HEADER_SIZE 10
#define FRAME_CONTENT 4096

typedef struct {
	char id[4];
	int major;
	int revision;
	char flags[4];
	int size;
} ID3Header;

typedef struct {
	char id[5];
	int size;
	char *text;
	int encoding;
} ID3Tag;

typedef struct {
	ID3Tag *tags;
	int count;
} ID3TagCollection;

typedef struct {
	char *name;
	char *parent;
	char *full_path;
	ID3Header header;
	ID3TagCollection tags;
} ID3FileRef;

void ID3readTags(ID3FileRef fileref, ID3TagCollection *tag);
void ID3readHeader(ID3FileRef fileref, ID3Header *header);
void ID3readFile(const char *path, ID3FileRef *fileref);

#endif
