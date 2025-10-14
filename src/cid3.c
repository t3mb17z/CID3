#include <utils/encoding.h>
#include <utils/errhandler.h>
#include <cid3.h>
#include <utils/syncsafe.h>
#include <utils/memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int openFile(FILE **fp, const char *path, const char *mode) {
  if(access(path, F_OK) != 0) {
    return 0;
  }
  *fp = fopen(path, mode);
  if(*fp == NULL) {
    fprintf(stderr, "file '%s' doesn't exists\n", path);
    exit(1);
  }
  return 1;
}

int CID3_ReadTags(CID3_FileRef **fileref, CID3_TagCollection *tag) {
  FILE *fp; openFile(&fp, (*fileref)->full_path, "rb");
  fseek(fp, 10, SEEK_SET);

  tag->count = 0;

  size_t bytes_read = 0, header_size, data_size;
  int frame_size;
  unsigned char *header, *data;

  while(bytes_read < (*fileref)->header.size) {
  	header = calloc(HEADER_SIZE + 1, sizeof(unsigned char));
  	bytes_read += header_size = fread(header, sizeof(unsigned char), HEADER_SIZE, fp);
  	if(header_size != HEADER_SIZE) {
  		CID3_SetError("Ocurrió un error\n");
      free(header);
      fclose(fp);
  		return 0;
    }
    frame_size = CID3_SyncSafeToInt(&header[4]);
    tag->tags[tag->count].size = frame_size;
    if(frame_size >= (*fileref)->header.size || frame_size <= 0) {
      free(header);
      fclose(fp);
      return 0;
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
      CID3_ProccessFrame((const char *)&data[1], data_size, &(tag->tags[tag->count].text));
    }

    tag->count++;
    
  	free(header);
    free(data);
  }
  fclose(fp);
  return 1;
}

int CID3_ReadHeader(CID3_FileRef **fileref, CID3_Header *header) {
  FILE *fp = fopen((*fileref)->full_path, "rb");
  if(fp == NULL) {
    CID3_SetError("Could not open file");
    return 0;
  }
  size_t bytes_read;
  unsigned char *buffer = calloc(1, HEADER_SIZE);

  bytes_read = fread(buffer, 1, HEADER_SIZE, fp);
  if(bytes_read != HEADER_SIZE) {
    CID3_SetError("Could not read header\n");
    return 0;
  }
  strncpy(header->id, (const char *)buffer, 3);
  header->id[3] = '\0';
  header->major = buffer[3];
  header->revision = buffer[4];
  header->size = CID3_SyncSafeToInt32(&buffer[6]);
  strncpy(header->flags, (char *)&buffer[5], 3);
  header->flags[3] = '\0';
  fclose(fp);
  return 1;
}

int CID3_ReadFile(const char *path, CID3_FileRef **fileref) {
  CID3_Header header;
  CID3_TagCollection tags;
  tags.tags = CID3_Alloc(100, sizeof(CID3_Tag), "Allocation for tags failed");
  if(tags.tags == NULL) return 0;
  (*fileref)->full_path = CID3_Alloc(1, 4096, "Could not allocate memory");
  if((*fileref)->full_path == NULL) {
    free(tags.tags);
    return 0;
  }
  strncpy((*fileref)->full_path, path, strlen(path));
  if(!CID3_ReadHeader(fileref, &header)) {
    free(tags.tags);
    free((*fileref)->full_path);
    return 0;
  }
  (*fileref)->header = header;
  if(!CID3_ReadTags(fileref, &tags)) {
    free(tags.tags);
    free((*fileref)->full_path);
    return 0;
  }
  (*fileref)->tags = tags;
  return 1;
}
