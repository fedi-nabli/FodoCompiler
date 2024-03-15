#ifndef FODOCOMPILER_CODEGEN_H
#define FODOCOMPILER_CODEGEN_H

#include "compiler.h"

#define STACK_PUSH_SIZE 4

// < Stack frame flags enum start

enum
{
  STACK_FRAME_ELEMENT_FLAG_IS_PUSHED_ADDRESS = 0b00000001,
  STACK_FRAME_ELEMENT_FLAG_ELEMENT_NOT_FOUND = 0b00000010,
  STACK_FRAME_ELEMENT_FLAG_IS_NUMERICAL = 0b00000100,
  STACK_FRAME_ELEMENT_FLAG_HAS_DATATYPE = 0b00001000
};

// > Stack frame flags enum end

// < Stack frame element types enum start

enum
{
  STACK_FRAME_ELEMENT_TYPE_LOCAL_VARIABLE,
  STACK_FRAME_ELEMENT_TYPE_SAVED_REGISTER,
  STACK_FRAME_ELEMENT_TYPE_SAVED_BP,
  STACK_FRAME_ELEMENT_TYPE_PUSHED_VALUE,
  STACK_FRAME_ELEMENT_TYPE_UNKNOWN
};

// > Stack frame element types enum end

struct codegen_entry_point
{
  // ID of the entry point
  int id;
};

struct codegen_exit_point
{
  // ID of the exit point
  int id;
};

struct string_table_element
{
  // This is the string that the element is related to
  const char* str;

  // This is the assembly label that points to the memory
  // where the string can be found
  const char label[50];
};

struct stack_frame_data
{
  // The datatype that was pushed to the stack
  struct datatype dtype;
};

struct stack_frame_element
{
  // Stack frame element flags
  int flags;
  // Stack frame element element type
  int type;

  // The name of the frame element, not a variable name, I.e result_value
  const char* name;

  // The offset this element is on the base pointer
  int offset_from_bp;

  struct stack_frame_data data;
};

// < Stackframe function helpers start

void stackframe_peek_start(struct node* func_node);
struct stack_frame_element* stackframe_peek(struct node* func_node);
struct stack_frame_element* stackframe_back(struct node* func_node);
struct stack_frame_element* stackframe_back_expect(struct node* func_node, int expecting_type, const char* expecting_name);
void stackframe_push(struct node* func_node, struct stack_frame_element* element);
void stackframe_pop(struct node* func_node);
void stackframe_pop_expecting(struct node* func_node, int expecting_type, const char* expecting_name);
void stackframe_sub(struct node* func_node, int type, const char* name, size_t amount);
void stackframe_add(struct node* func_node, int type, const char* name, size_t amount);
void stackframe_assert_empty(struct node* func_node);

// > Stackframe function helpers end

#endif