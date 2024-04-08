#ifndef STDIO_H
#define STDIO_H

#include <stdlib.h>

typedef struct _iobuf
{
  char* _ptr;
  int _cnt;
  char* _base;
  int _flags;
  int _file;
  int _charbuf;
  int _bufsize;
  char* _tmpfname;
} FILE;

int printf(const char* fmt, ...);
FILE* fopen(const char* filename, const char* mode);
size_t fwrite(const char* ptr, size_t size, size_t nmemb, FILE* stream);
size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
int fclose(FILE* stream);

#endif