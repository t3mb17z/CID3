#include "utils/encoding.h"
#include <id3.h>
#include <utils/syncsafe.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void openFile(FILE **fp, const char *path, const char *mode) {
  if(access(path, F_OK) != 0) {
    return;
  }
  *fp = fopen(path, mode);
  if(*fp == NULL) {
    fprintf(stderr, "file '%s' doesn't exists\n", path);
    exit(1);
  }
}

void ID3readTags(ID3FileRef fileref, ID3TagCollection *tag) {
  FILE *fp; openFile(&fp, fileref.full_path, "rb");
  fseek(fp, 10, SEEK_SET);

  tag->count = 0;

  size_t bytes_read = 0, header_size, data_size;
  int frame_size;
  unsigned char *header, *data;

  while(bytes_read < fileref.header.size) {
  	header = calloc(HEADER_SIZE + 1, sizeof(unsigned char));
  	bytes_read += header_size = fread(header, sizeof(unsigned char), HEADER_SIZE, fp);
  	if(header_size != HEADER_SIZE) {
  		fprintf(stderr, "Ocurrió un error\n");
      free(header);
      fclose(fp);
  		return;
    }
    frame_size = ID3_sync_safe_to_int32(&header[4]);
    tag->tags[tag->count].size = frame_size;
    if(frame_size >= fileref.header.size || frame_size <= 0) {
      free(header);
      fclose(fp);
      return;
    }
    strncpy(tag->tags[tag->count].id, (const char *)header, 4);
    data = calloc(frame_size, 1);
    bytes_read += data_size = fread(data, sizeof(unsigned char), frame_size, fp);
    if(strncmp((const char *)header, "APIC", 4) == 0) {
      fseek(fp, frame_size, SEEK_CUR);
      continue;
    }
    tag->tags[tag->count].text = calloc(frame_size * 1000, 1);
    if(data[0] == 0x00 || data[0] == 0x03) {
      strncpy(tag->tags[tag->count].text, (const char *)&data[1], frame_size - 1);
    } else if(data[0] == 0x01 || data[0] == 0x02) {
      ID3proccessFrame((const char *)&data[1], data_size, &(tag->tags[tag->count].text));
    }

    tag->count++;
    
  	free(header);
    free(data);
  }
  fclose(fp);
}

void ID3readHeader(ID3FileRef fileref, ID3Header *header) {
  FILE *fp = fopen(fileref.full_path, "rb");
  if(fp == NULL) {
    perror("fopen");
    exit(1);
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
  header->size = ID3_sync_safe_to_int32(&buffer[6]);
  strncpy(header->flags, (char *)&buffer[5], 3);
  header->flags[3] = '\0';
  fclose(fp);
}

void ID3readFile(const char *path, ID3FileRef *fileref) {
  ID3Header header;
  ID3TagCollection tags;
  tags.tags = calloc(100, sizeof(ID3Tag));
  fileref->full_path = calloc(1, 4096);
  strncpy(fileref->full_path, path, strlen(path));
  ID3readHeader(*fileref, &header);
  fileref->header = header;
  ID3readTags(*fileref, &tags);
  fileref->tags = tags;
}
