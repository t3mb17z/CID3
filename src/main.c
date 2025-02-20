#include <id3utils.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	ID3FileRef file;
	ID3Header header;
	ID3TagCollection tags;

	ID3readFile(argv[1], &file, &header, &tags);

	for(int i = 0; i < tags.count; i++)
		printf("%s: %s, %d\n", tags.tags[i].id, tags.tags[i].text, tags.tags[i].size);
	
	return 0;
}
