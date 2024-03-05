#ifndef FODOCOMPILER_CODEGEN_H
#define FODOCOMPILER_CODEGEN_H

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

#endif