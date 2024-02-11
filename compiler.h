#ifndef FODOCOMPILER_H
#define FODOCOMPILER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define S_EQ(str, str2) \
        (str && str2 && (strcmp(str, str2) == 0))

// Lexer definitions
#define NUMERIC_CASE  \
  case '0':           \
  case '1':           \
  case '2':           \
  case '3':           \
  case '4':           \
  case '5':           \
  case '6':           \
  case '7':           \
  case '8':           \
  case '9'

#define OPERATOR_CASE_EXCLUDING_DIVISION  \
  case '+':                               \
  case '-':                               \
  case '*':                               \
  case '>':                               \
  case '<':                               \
  case '^':                               \
  case '%':                               \
  case '!':                               \
  case '=':                               \
  case '~':                               \
  case '|':                               \
  case '&':                               \
  case '(':                               \
  case '[':                               \
  case ',':                               \
  case '.':                               \
  case '?'

#define SYMBOL_CASE \
  case '{':         \
  case '}':         \
  case ':':         \
  case ';':         \
  case '#':         \
  case '\\':        \
  case ')':         \
  case ']'

// Lexer case defintions end

enum
{
  COMPILER_FILE_COMPILED_OK,
  COMPILER_FAILED_WITH_ERRORS
};

// Lexer enums
enum
{
  LEXICAL_ANALYSIS_ALL_OK,
  LEXICAL_ANALYSIS_INPUT_ERROR
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
  struct pos pos;

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

struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process* lex_process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process* lex_process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process* lex_process, char c);

struct lex_process_functions
{
  LEX_PROCESS_NEXT_CHAR next_char;
  LEX_PROCESS_PEEK_CHAR peek_char;
  LEX_PROCESS_PUSH_CHAR push_char;
};

struct lex_process
{
  struct pos pos;
  struct vector* token_vec;
  struct compile_process* compiler;

  // ((50))
  int current_expression_count;

  struct buffer* parentheses_buffer;
  struct lex_process_functions* function;

  // This will be private data that the lexer does not understand
  // but the person using the lexer does understand
  void* private;
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

// Compiler error & warning functions
void compiler_error(struct compile_process* compiler, const char* msg, ...);
void compiler_warning(struct compile_process* compiler, const char* msg, ...);

int compile_file(const char* filename, const char* out_filename, int flags);

struct compile_process* compile_process_create(const char* filename, const char* filename_out, int flags);

// Compiler lex_process_functions from cprocess
char compile_process_next_char(struct lex_process* lex_process);
char compile_process_peek_char(struct lex_process* lex_process);
void compile_process_push_char(struct lex_process* lex_process, char c);

// Lexer functions
int lex(struct lex_process* process);

// Lexer process functions
struct lex_process* lex_process_create(struct compile_process* compiler, struct lex_process_functions* functions, void* private);
void lex_process_free(struct lex_process* process);
void* lex_process_private(struct lex_process* process);
struct vector* lex_process_tokens(struct lex_process* process);

// Token helper functions
bool token_is_keyword(struct token* token, const char* value);

#endif