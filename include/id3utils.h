#ifndef ID3UTILS_H
#define ID3UTILS_H

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
	int encoding;
	char *text;
} ID3Tag;

typedef struct {
	ID3Tag title;
} TagCollection;

typedef struct {
	char *name;
	char *parent;
	char *full_path;
	ID3Header header;
	ID3Tag tags[6];
} ID3FileRef;

void ID3readTags(ID3FileRef fileref, ID3Tag **tag);
void ID3readHeader(ID3FileRef fileref, ID3Header *header);
void ID3readFile(const char *path, ID3FileRef *fileref, ID3Header *header, ID3Tag *tags[]);

#endif
