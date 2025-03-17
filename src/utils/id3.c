#include "utils/id3.h"
#include "utils/str.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  int frame_size = 0, from_tag = 0;
  size_t bytes_read = 0;
  size_t header = 0, rframe_size;
  unsigned char *buffer;
  char *iso, *utf8data;
  uint16_t *data = calloc(4097, 1);
  tags->tags = calloc(128, sizeof(ID3Tag));
  tags->count = 0;

  fseek(fp, HEADER_SIZE, SEEK_SET);
  while(bytes_read < fileref.header.size) {
    buffer = calloc(HEADER_SIZE, 1);
    bytes_read += header = fread(buffer, 1, HEADER_SIZE, fp);
    if(header != HEADER_SIZE) {
      fprintf(stderr, "Is not a header, closing...\n");
      return;
    }
    if(strcmp((char *)buffer, "") == 0)
      continue;

    strncpy(tags->tags[tags->count].id, (char *)buffer, 4);
    tags->tags[tags->count].id[4] = '\0';

    frame_size = ID3_sync_safe_to_int32(&buffer[4]);
    buffer = realloc(buffer, frame_size);

    tags->tags[tags->count].size = frame_size;
    tags->tags[tags->count].text = calloc(frame_size, 1);

    bytes_read += from_tag = fread(buffer, 1, frame_size, fp);
    if(from_tag != frame_size) {
      fprintf(stderr, "Was not the exact frame size\n");
      return;
    }

    if(frame_size < 4096) {
      int encoding = buffer[0];
      if(encoding == 1 || encoding == 2) {
        memcpy(data, buffer, frame_size);
        rframe_size = frame_size;
        int endianness = detect_endianness(data, &rframe_size);
        data++;
        if(data[0] == 0xFE01 || data[0] == 0xFEFF)
          data++;
        if(data[0] == 0xFF)
          data++;
        ID3utf16_to_utf8(data, rframe_size, utf8data, endianness);
        strcpy(tags->tags[tags->count].text, utf8data);
      } else if(encoding == 0) {
        iso = calloc(frame_size, 1);
        for(int i = 0; i < frame_size; i++) {
          int chr = buffer[i];
          strcat(iso, (char *)&chr);
        }
        // ID3iso8859_1_to_utf8((char *)(buffer++), iso);
        strcpy(tags->tags[tags->count].text, (char *)iso);
      } else if(encoding == 3) {
        strcpy(tags->tags[tags->count].text, (char *)buffer);
      }
      tags->tags[tags->count].encoding = buffer[0];
      tags->count++;
    }

    free(buffer);

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

void ID3readFile(const char *path, ID3FileRef *fileref) {
  ID3Header header;
  ID3TagCollection tags;
  fileref->full_path = calloc(1, 4096);
  strcpy(fileref->full_path, path);
  ID3readHeader(*fileref, &header);
  fileref->header = header;
  ID3readTags(*fileref, &tags);
  fileref->tags = tags;
}
