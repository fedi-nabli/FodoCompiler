#include "compiler.h"
#include "helpers/vector.h"

#include <stdio.h>
#include <stdarg.h>

static struct compile_process* current_process = NULL;

// < Codegen scope functions start

void codegen_new_scope(int flags)
{
  #warning "The resolver needs to exist for this to work"
}

void codegen_finish_scope()
{
  #warning "The resolver needs to exist for this to work"
}

// > Codegen scope functions end

struct node* codegen_node_next()
{
  return vector_peek_ptr(current_process->node_tree_vec);
}

void asm_push_args(const char* ins, va_list args)
{
  va_list args2;
  va_copy(args2, args);
  vfprintf(stdout, ins, args);
  fprintf(stdout, "\n");
  if (current_process->ofile)
  {
    vfprintf(current_process->ofile, ins, args);
    fprintf(current_process->ofile, "\n");
  }
}

void asm_push(const char* ins, ...)
{
  va_list args;
  va_start(args, ins);
  asm_push_args(ins, args);
  va_end(args);
}

void codegen_generate_data_section_part(struct node* node)
{
  #warning "Create a switch here for processing the global data"
}

void codegen_generate_data_section()
{
  asm_push("section .data");
  struct node* node = codegen_node_next();
  while (node)
  {
    codegen_generate_data_section_part(node);
    node = codegen_node_next();
  }
}

void codegen_generate_root_node(struct node* node)
{
  #warning "Process any functions"
}

void codegen_generate_root()
{
  asm_push("section .text");
  struct node* node = NULL;
  while ((node = codegen_node_next()) != NULL)
  {
    codegen_generate_root_node(node);
  }
}

void codegen_write_strings()
{
  #warning "Loop through the string table and write all the strings"
}

void codegen_generate_rod()
{
  asm_push("section .rodata");
  codegen_write_strings();
}

int codegen(struct compile_process* process)
{
  current_process = process;
  scope_create_root(process);
  vector_set_peek_pointer(process->node_tree_vec, 0);
  codegen_new_scope(0);
  codegen_generate_data_section();
  vector_set_peek_pointer(process->node_tree_vec, 0);
  codegen_generate_root();
  codegen_finish_scope();

  // Generate read only data
  codegen_generate_rod();

  return CODEGEN_ALL_OK;
}