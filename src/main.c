#include <id3utils.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	ID3FileRef file;

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
