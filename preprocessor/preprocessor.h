#ifndef FODOCOMPILER_PREPROCESSOR_H
#define FODOCOMPILER_PREPROCESSOR_H

#ifdef _WIN32
  #include <windows.h>
  #define PATH_MAX MAX_PATH
#elif __linux__
  #include <linux/limits.h>
#elif __APPLE__
  #include <limits.h>
#endif

enum
{
  PREPROCESSOR_DEFINITION_STANDARD,
  PREPROCESSOR_DEFINITION_MACRO_FUNCTION,
  PREPROCESSOR_DEFINITION_NATIVE_CALLBACK,
  PREPROCESSOR_DEFINITION_TYPEDEF
};

struct preprocessor;
struct preprocessor_definition;
struct preprocessor_included_file;
struct preprocessor_function_argument;
struct preprocessor_function_arguments;

typedef void (*PREPROCESSOR_STATIC_INCLUDE_HANDLER_POST_CREATION)(struct preprocessor* preprocessor, struct preprocessor_included_file* included_file);
typedef int (*PREPROCESSOR_DEFINITION_NATIVE_CALL_EVALUATION)(struct preprocessor_definition* definition, struct preprocessor_function_arguments* arguments);
typedef struct vector* (*PREPROCESSOR_DEFINITION_NATIVE_CALL_VALUE)(struct preprocessor_definition* definition, struct preprocessor_function_arguments* arguments);

// < Preprocessor structures start

struct preprocessor_function_argument
{
  // Tokens for this argument
  struct vector* tokens;
};

struct preprocessor_function_arguments
{
  // A vector of struct preprocessor_function_argument
  struct vector* arguments;
};

struct preprocessor_definition
{
  // i.e standard or macro function
  int type;

  // The name. i.e "#define ABC", ABC is the name
  const char* name;

  union
  {
    struct standard_preprocessor_definition
    {
      // A vector of defintions value tokens. Values can be multiple lines
      // vector of struct token
      struct vector* value;

      // A vector of const char* respresenting function arguments in order
      // for example: ABC(a, b, c)
      struct vector* arguments;
    } standard;

    struct typedef_preprocessor_definition
    {
      struct vector* value;
    } _typedef;

    struct native_callback_preprocessor_definition
    {
      PREPROCESSOR_DEFINITION_NATIVE_CALL_EVALUATION evaluate;
      PREPROCESSOR_DEFINITION_NATIVE_CALL_VALUE value;
    } native;
  };

  struct preprocessor* preprocessor;
};

struct preprocessor_included_file
{
  char filename[PATH_MAX];
};

struct preprocessor
{
  // A vector of struct preprocessor_definition*
  struct vector* definitions;
  // Vector of struct preprocessor_node*
  struct vector* exp_vector;

  struct expressionable* expressionable;

  struct compile_process* compiler;

  // A vector of included files: struct preprocessor_included_file*
  struct vector* includes;
};

// > Preprocessor structures end

// Preprocessor native funtions
void preprocessor_create_definitions(struct preprocessor* preprocessor);

// Preprocessor global functions
struct token* preprocessor_previous_token(struct compile_process* compiler);
struct vector* preprocessor_build_value_vector_for_integer(int value);
struct preprocessor_definition* preprocessor_definition_create_native(struct preprocessor* preprocessor, const char* name, PREPROCESSOR_DEFINITION_NATIVE_CALL_EVALUATION evaluate, PREPROCESSOR_DEFINITION_NATIVE_CALL_VALUE value);

PREPROCESSOR_STATIC_INCLUDE_HANDLER_POST_CREATION preprocessor_static_include_handler_for(const char* filename);

#endif