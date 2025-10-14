#include <stdio.h>
#include <cid3/cid3.h>

int main(int argc, char *argv[]) {

  CID3_FileRef *file;

  if (argc < 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }
  
  CID3_ReadFile(argv[1], &file);

	for (int i = 0; i < file->tags.count; i++) {
		printf("Tag: %s\n", file->tags.tags[i].text);
	}

  return 0;
}
