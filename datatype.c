#include "compiler.h"

bool datatype_is_struct_or_union_for_name(const char* name)
{
  return S_EQ(name, "union") || S_EQ(name, "struct");
}

bool datatype_is_struct_or_union(struct datatype* dtype)
{
  return dtype->type == DATA_TYPE_STRUCT || dtype->type == DATA_TYPE_UNION;
}

bool datatype_is_primitive(struct datatype* dtype)
{
  return !datatype_is_struct_or_union(dtype);
}

bool datatype_is_struct_or_union_non_pointer(struct datatype* dtype)
{
  return dtype->type != DATA_TYPE_UNKNOWN && !datatype_is_primitive(dtype) && !(dtype->flags & DATATYPE_FLAG_IS_POINTER);
}

bool datatype_is_void_no_ptr(struct datatype* dtype)
{
  return S_EQ(dtype->type_str, "void") && !(dtype->flags & DATATYPE_FLAG_IS_POINTER);
}

size_t datatype_size(struct datatype* dtype)
{
  if (dtype->flags & DATATYPE_FLAG_IS_POINTER && dtype->pointer_depth > 0)
  {
    return DATA_SIZE_DWORD;
  }

  if (dtype->flags & DATATYPE_FLAG_IS_ARRAY)
  {
    return dtype->array.size;
  }

  return dtype->size;
}

size_t datatype_size_for_array_access(struct datatype* dtype)
{
  if (datatype_is_struct_or_union(dtype) && dtype->flags & DATATYPE_FLAG_IS_POINTER && dtype->pointer_depth == 1)
  {
    // struct abc* abc; abc[0];
    return dtype->size;
  }

  return datatype_size(dtype);
}

size_t datatype_element_size(struct datatype* dtype)
{
  if (dtype->flags & DATATYPE_FLAG_IS_POINTER)
  {
    return DATA_SIZE_DWORD;
  }

  return dtype->size;
}

size_t datatype_size_no_ptr(struct datatype* dtype)
{
  if (dtype->flags & DATATYPE_FLAG_IS_ARRAY)
  {
    return dtype->array.size;
  }

  return dtype->size;
}

struct datatype datatype_for_numeric()
{
  struct datatype dtype = {};
  dtype.flags |= DATATYPE_FLAG_IS_LITERAL;
  dtype.type = DATA_TYPE_INTEGER;
  dtype.type_str = "int";
  dtype.size = DATA_SIZE_DWORD;
  return dtype;
}

struct datatype datatype_for_string()
{
  struct datatype dtype = {};
  dtype.type = DATA_TYPE_INTEGER;
  dtype.type_str = "char";
  dtype.flags |= DATATYPE_FLAG_IS_POINTER | DATATYPE_FLAG_IS_LITERAL;
  dtype.pointer_depth = 1;
  dtype.size = DATA_SIZE_DWORD;
  return dtype;
}

void datatype_set_void(struct datatype* dtype)
{
  dtype->type = DATA_TYPE_VOID;
  dtype->type_str = "void";
  dtype->size = DATA_SIZE_ZERO;
}