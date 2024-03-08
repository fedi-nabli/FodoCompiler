#include "compiler.h"
#include "helpers/vector.h"

#include <assert.h>

size_t variable_size(struct node* var_node)
{
  assert(var_node->type == NODE_TYPE_VARIABLE);
  return datatype_size(&var_node->var.type);
}

size_t variable_size_for_list(struct node* var_list_node)
{
  assert(var_list_node->type == NODE_TYPE_VARIABLE_LIST);
  size_t size = 0;
  vector_set_peek_pointer(var_list_node->var_list.list, 0);
  struct node* var_node = vector_peek_ptr(var_list_node->var_list.list);
  while (var_node)
  {
    size += variable_size(var_node);
    var_node = vector_peek_ptr(var_list_node->var_list.list);
  }

  return size;
}

struct node* variable_struct_or_union_body_node(struct node* node)
{
  if (!node_is_struct_or_union_variable(node))
  {
    return NULL;
  }

  if (node->var.type.type == DATA_TYPE_STRUCT)
  {
    return node->var.type.struct_node->_struct.body_n;
  }

  // return the unions body
  if (node->var.type.type == DATA_TYPE_UNION)
  {
    return node->var.type.union_node->_union.body_n;
  }

  return NULL;
}

int padding(int val, int to)
{
  if (to <= 0)
  {
    return 0;
  }

  if ((val % to) == 0)
  {
    return 0;
  }

  return to - (val % to) % to;
}

int align_value(int val, int to)
{
  if (val % to)
  {
    val += padding(val, to);
  }

  return val;
}

int align_value_treat_positive(int val, int to)
{
  assert(to >= 0);
  if (val < 0)
  {
    to = -to;
  }

  return align_value(val, to);
}

int compute_sum_padding(struct vector* vec)
{
  int padding = 0;
  int last_type = -1;
  bool mixed_types = false;
  vector_set_peek_pointer(vec, 0);
  struct node* cur_node = vector_peek_ptr(vec);
  struct node* last_node = NULL;
  while (cur_node)
  {
    if (cur_node->type != NODE_TYPE_VARIABLE)
    {
      cur_node = vector_peek_ptr(vec);
      continue;
    }

    padding += cur_node->var.padding;
    last_type = cur_node->var.type.type;
    last_node = cur_node;
    cur_node = vector_peek_ptr(vec);
  }

  return padding;
}

int array_multiplier(struct datatype* dtype, int index, int index_value)
{
  if (!(dtype->flags & DATATYPE_FLAG_IS_ARRAY))
  {
    return index_value;
  }

  vector_set_peek_pointer(dtype->array.brackets->n_brackets, index + 1);
  int size_sum = index_value;
  struct node* bracket_node = vector_peek_ptr(dtype->array.brackets->n_brackets);
  while (bracket_node)
  {
    assert(bracket_node->bracket.inner->type == NODE_TYPE_NUMBER);
    int declared_index = bracket_node->bracket.inner->llnum;
    int size_value = declared_index;
    size_sum *= size_value;
    bracket_node = vector_peek_ptr(dtype->array.brackets->n_brackets);
  }

  return size_sum;
}

int array_offset(struct datatype* dtype, int index, int index_value)
{
  if (!(dtype->flags & DATATYPE_FLAG_IS_ARRAY) ||
      (index == vector_count(dtype->array.brackets->n_brackets) - 1))
  {
    return index_value * datatype_element_size(dtype);
  }

  return array_multiplier(dtype, index, index_value) * datatype_element_size(dtype);
}