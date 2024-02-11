#ifndef FODOCOMPILER_H
#define FODOCOMPILER_H

#include <stdio.h>
#include <stdbool.h>

enum
{
  COMPILER_FILE_COMPILED_OK,
  COMPILER_FAILED_WITH_ERRORS
};

// Token types enum
enum
{
  TOKEN_TYPE_IDENTIFIER,
  TOKEN_TYPE_KEYWORD,
  TOKEN_TYPE_OPERATOR,
  TOKEN_TYPE_SYMBOL,
  TOKEN_TYPE_NUMBER,
  TOKEN_TYPE_STRING,
  TOKEN_TYPE_COMMENT,
  TOKEN_TYPE_NEWLINE
};

struct pos
{
  int line;
  int col;
  const char* filename;
};

struct token
{
  int type;
  int flags;

  union
  {
    char cval;
    const char* sval;
    unsigned int inum;
    unsigned long lnum;
    unsigned long long llnum;
    void* any;
  };

  // True if there is whitespace between the token and the next token
  // i.e "* a" for operator token "*" would mean whitespace would be set for token "a"
  bool whitespace;

  // (5+10+20)
  const char* between_brackets;
};

struct compile_process
{
  // The flags in regards to how this file should be compiled
  int flags;

  struct pos pos;
  struct compile_process_input_file
  {
    FILE* fp;
    const char* abs_path;
  } cfile;

  FILE* ofile;
};

int compile_file(const char* filename, const char* out_filename, int flags);

struct compile_process* compile_process_create(const char* filename, const char* filename_out, int flags);

#endif