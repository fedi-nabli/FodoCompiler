#include "compiler.h"
#include "helpers/vector.h"

static void symresolver_push_symbol(struct compile_process* process, struct symbol* symbol)
{
  vector_push(process->symbols.table, &symbol);
}

void symresolver_initialize(struct compile_process* process)
{
  process->symbols.tables = vector_create(sizeof(struct vector*));
}

void symresolver_new_table(struct compile_process* process)
{
  // Save the current table
  vector_push(process->symbols.tables, &process->symbols.table);

  // Overwrite the active table
  process->symbols.table = vector_create(sizeof(struct symbol*));
}

void symresolver_end_table(struct compile_process* process)
{
  struct vector* last_table = vector_back_ptr(process->symbols.tables);
  process->symbols.table = last_table;
  vector_pop(process->symbols.tables);
}

struct symbol* symresolver_get_symbol(struct compile_process* process, const char* name)
{
  vector_set_peek_pointer(process->symbols.table, 0);
  struct symbol* symbol = vector_peek_ptr(process->symbols.table);
  while (symbol)
  {
    if (S_EQ(symbol->name, name))
    {
      break;
    }

    symbol = vector_peek_ptr(process->symbols.table);
  }

  return symbol;
}

struct symbol* symresolver_get_symbol_for_native_function(struct compile_process* process, const char* name)
{
  struct symbol* symbol = symresolver_get_symbol(process, name);
  if (!symbol)
  {
    return NULL;
  }

  if (symbol->type != SYMBOL_TYPE_NATIVE_FUNCTION)
  {
    return NULL;
  }

  return symbol;
}

struct symbol* symresolver_register_symbol(struct compile_process* process, const char* sym_name, int type, void* data)
{
  if (symresolver_get_symbol(process, sym_name))
  {
    return NULL;
  }

  struct symbol* symbol = calloc(1, sizeof(struct symbol));
  symbol->name = sym_name;
  symbol->type = type;
  symbol->data = data;
  symresolver_push_symbol(process, symbol);
  return symbol;
}

struct node* symresolver_node(struct symbol* symbol)
{
  if (symbol->type != SYMBOL_TYPE_NODE)
  {
    return NULL;
  }

  return symbol->data;
}

void symresolver_build_for_variable_node(struct compile_process* process, struct node* node)
{
  compiler_error(process, "Variables are not yet supported\n");
}

void symresolver_build_for_function_node(struct compile_process* process, struct node* node)
{
  compiler_error(process, "Functions are not yet supported\n");
}

void symresolver_build_for_structure_node(struct compile_process* process, struct node* node)
{
  if (node->flags & NODE_FLAG_IS_FORWARD_DECLARATION)
  {
    // We do not register forward declarations
    return;
  }

  symresolver_register_symbol(process, node->_struct.name, SYMBOL_TYPE_NODE, node);
}

void symresolver_build_for_union_node(struct compile_process* process, struct node* node)
{
  compiler_error(process, "Unions are not yet supported\n");
}

void symresolver_build_for_node(struct compile_process* process, struct node* node)
{
  switch (node->type)
  {
    case NODE_TYPE_VARIABLE:
      symresolver_build_for_variable_node(process, node);
      break;

    case NODE_TYPE_FUNCTION:
      symresolver_build_for_function_node(process, node);
      break;

    case NODE_TYPE_STRUCT:
      symresolver_build_for_structure_node(process, node);
      break;

    case NODE_TYPE_UNION:
      symresolver_build_for_union_node(process, node);
      break;
  }

  // Ignore all other node type, because they can't become symbols
}