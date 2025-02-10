#include <id3utils.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	if(argc < 2) {
		fprintf(stderr, "Needs argument to work");
		return 1;
	}

	ID3FileRef fileref;
	ID3Header header;
	ID3Tag *tags;

	ID3readFile(argv[1], &fileref, &header, &tags);

	return 0;
}
