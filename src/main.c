#include <id3utils.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	ID3FileRef file;
	ID3Header header;
	ID3TagCollection tags;

	ID3readFile(argv[1], &file, &header, &tags);

	printf("Title: %s", file.tags.title.text);
	
	return 0;
}
