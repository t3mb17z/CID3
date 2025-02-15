#include <id3utils.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strutils.h>

int ID3_sync_safe_to_int(unsigned char *syncsafe) {
  return (
    (syncsafe[0] << 21) |
    (syncsafe[1] << 14) |
    (syncsafe[2] << 7) |
    (syncsafe[3])
  );
};

int ID3_sync_safe_to_int32(unsigned char *syncsafe) {
  return (
    (syncsafe[0] << 24) |
    (syncsafe[1] << 16) |
    (syncsafe[2] << 8) |
    (syncsafe[3])
  );
};

void ID3readTags(ID3FileRef fileref, ID3TagCollection *tags) {
  FILE *fp = fopen(fileref.full_path, "rb");
  if(fp == NULL) {
    perror("fopen");
    return;
  }
  int frame_size = 0;
  size_t bytes_read = 0;
  size_t from_tag = 0, header = 0, rframe_size;
  unsigned char *buffer;
  uint16_t *data = calloc(1, 1024);
  ID3Tag result;

  fseek(fp, HEADER_SIZE, SEEK_SET);
  while(bytes_read < fileref.header.size) {
    buffer = calloc(1, fileref.header.size);
    bytes_read += header = fread(buffer, 1, HEADER_SIZE, fp);
    if(header != HEADER_SIZE) {
      fprintf(stderr, "Is not a header, closing...\n");
      return;
    }
		if(strcmp((char *)buffer, "") == 0)
			continue;

    strncpy(result.id, (char *)buffer, 4);
    result.id[4] = '\0';

    frame_size = ID3_sync_safe_to_int32(&buffer[4]);

    result.size = frame_size;
    result.text = calloc(1, 256);
    
    bytes_read += from_tag = fread(buffer, 1, frame_size, fp);
    if(frame_size < 4096 && ((int)buffer[1] == 0xFE || (int)buffer[1] == 0xFF)) {
      memcpy(data, buffer, frame_size);
      data++;
      if(data[0] == 0xFEFF)
        data++;
      if(data[0] == 0xFF)
        data++;
      rframe_size = frame_size;
      int endianness = detect_endianness(data, &rframe_size);
      ID3utf16_to_utf8(data, rframe_size, result.text, endianness);
    } else {
      strcpy(result.text, (char *)buffer);
    }

    if(strcmp(result.id, "TIT2") == 0) {
      tags->title = result;
    } else if(strcmp(result.id, "TPE1") == 0) {
      tags->artist = result;
    } else if(strcmp(result.id, "TALB") == 0) {
      tags->album = result;
    }

  }
  fclose(fp);
}

void ID3readHeader(ID3FileRef fileref, ID3Header *header) {
  FILE *fp = fopen(fileref.full_path, "rb");
  if(fp == NULL) {
    perror("fopen");
    return;
  }
  size_t bytes_read;
  unsigned char *buffer = calloc(1, HEADER_SIZE);

  bytes_read = fread(buffer, 1, HEADER_SIZE, fp);
  if(bytes_read != HEADER_SIZE) {
    fprintf(stderr, "Could not read header\n");
    return;
  }
  strncpy(header->id, (const char *)buffer, 3);
  header->id[3] = '\0';
  header->major = buffer[3];
  header->revision = buffer[4];
  header->size = ID3_sync_safe_to_int(&buffer[6]);
  strncpy(header->flags, (char *)&buffer[5], 3);
  header->flags[3] = '\0';
  fclose(fp);
}

void ID3readFile(const char *path, ID3FileRef *fileref, ID3Header *header, ID3TagCollection *tags) {
  fileref->full_path = calloc(1, 4096);
  strcpy(fileref->full_path, path);
  ID3readHeader(*fileref, header);
  fileref->header = *header;
  ID3readTags(*fileref, tags);
  fileref->tags = *tags;
}
