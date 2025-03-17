#include <utils/id3.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	ID3FileRef file;

  if(argc < 2) {
    fprintf(stderr, "Usage: %s </path/to/file>\n", argv[0]);
    exit(0);
  }

	ID3readFile(argv[1], &file);

	for(int i = 0; i < file.tags.count; i++)
		printf("%s: %s, %d, %d\n",
			file.tags.tags[i].id,
		  file.tags.tags[i].text,
			file.tags.tags[i].size,
			file.tags.tags[i].encoding
		);

	return 0;
}
