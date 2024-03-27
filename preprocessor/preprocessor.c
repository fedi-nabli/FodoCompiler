#include "compiler.h"
#include "expressionable.h"
#include "helpers/buffer.h"
#include "helpers/vector.h"

// < Preprocessor enums start

enum
{
  TYPEDEF_TYPE_STANDARD,
  // A structure typedef is basically "typedef struct abc { int x; } abc;"
  TYPEDEF_TYPE_STRUCTURE_TYPEDEF
};

enum
{
  PREPROCESSOR_NUMBER_NODE,
  PREPROCESSOR_IDENTIFIER_NODE,
  PREPROCESSOR_KEYWORD_NODE,
  PREPROCESSOR_UNARY_NODE,
  PREPROCESSOR_EXPRSSION_NODE,
  PREPROCESSOR_JOINED_NODE,
  PREPROCESSOR_TENARY_NODE
};

// > Preprocessor enums end

// < Preprocessor stuctures start

struct typdef_type
{
  int type;
  const char* definition_name;
  struct vector* value;
  struct typedef_structure
  {
    const char* sname;
  } structure;
};

struct preprocessor_node
{
  int type;
  struct preprocessor_const_val
  {
    union
    {
      char cval;
      unsigned int inum;
      long lnum;
      long long llnum;
      unsigned long ulnum;
      unsigned long long ullnum;
    };
  } const_val;

  union
  {
    struct preprocessor_exp_node
    {
      struct preprocessor_node* left_node;
      struct preprocessor_node* right_node;
      const char* op;
    } exp_node;

    struct preprocessor_unary_node
    {
      struct preprocessor_node* operand_node;
      const char* op;
      struct preprocessor_unary_indirection
      {
        int depth;
      } indirection;
    } unary_node;

    struct preprocessor_parenthesis_node
    {
      struct preprocessor_node* exp;
    } parenthesis_node;

    struct preprocessor_joined_node
    {
      struct preprocessor_node* left;
      struct preprocessor_node* right;
    } joined_node;

    struct preprocessor_tenary_node
    {
      struct preprocessor_node* true_node;
      struct preprocessor_node* false_node;
    } tenary_node;
  };

  const char* sval;
};

// > Preprocessor structures end