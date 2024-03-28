#include "compiler.h"
#include "expressionable.h"
#include "helpers/buffer.h"
#include "helpers/vector.h"
#include "preprocessor/preprocessor.h"

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
  PREPROCESSOR_PARENTHESES_NODE,
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

// < Preprocessor initializer functions start

void preprocessor_initialize(struct preprocessor* preprocessor)
{
  memset(preprocessor, 0, sizeof(struct preprocessor));
  preprocessor->definitions = vector_create(sizeof(struct preprocessor_definition*));
  preprocessor->includes = vector_create(sizeof(struct preprocessor_included_file*));
  #warning "Create preprocessor default definitions"
}

struct preprocessor* preprocessor_create(struct compile_process* compiler)
{
  struct preprocessor* preprocessor = calloc(1, sizeof(struct preprocessor));
  preprocessor_initialize(preprocessor);
  preprocessor->compiler = compiler;
  return preprocessor;
}

// > Preprocessor initialize functions end

void preprocessor_execute_warning(struct compile_process* compiler, const char* msg)
{
  compiler_warning(compiler, "#warning %s", msg);
}

void preprocessor_execute_error(struct compile_process* compiler, const char* msg)
{
  compiler_error(compiler, "#error %s", msg);
}

bool preprocessor_is_keyword(const char* type)
{
  return S_EQ(type, "defined");
}

struct token* preprocessor_next_token(struct compile_process* compiler)
{
  return vector_peek(compiler->token_vec_original);
}

void* preprocessor_node_create(struct preprocessor_node* node)
{
  struct preprocessor_node* result = calloc(1, sizeof(struct preprocessor_node));
  memcpy(result, node, sizeof(struct preprocessor_node));
  return result;
}

// < Preprocessor expressionable system start

void* preprocessor_handle_number_token(struct expressionable* expressionable)
{
  struct token* token = expressionable_token_next(expressionable);
  return preprocessor_node_create(&(struct preprocessor_node){.type=PREPROCESSOR_NUMBER_NODE, .const_val.llnum=token->llnum});
}

void* preprocessor_handle_identifier_token(struct expressionable* expressionable)
{
  struct token* token = expressionable_token_next(expressionable);
  bool is_preprocessor_keyword = preprocessor_is_keyword(token->sval);
  int type = PREPROCESSOR_IDENTIFIER_NODE;
  if (is_preprocessor_keyword)
  {
    type = PREPROCESSOR_KEYWORD_NODE;
  }

  return preprocessor_node_create(&(struct preprocessor_node){.type=type, .sval=token->sval});
}

void preprocessor_make_unary_node(struct expressionable* expressionable, const char* op, void* right_operand_node_ptr)
{
  struct preprocessor_node* right_operand_node = right_operand_node_ptr;
  void* unary_node = preprocessor_node_create(&(struct preprocessor_node){.type=PREPROCESSOR_UNARY_NODE, .unary_node.op=op, .unary_node.operand_node=right_operand_node});
  expressionable_node_push(expressionable, unary_node);
}

void preprocessor_make_unary_indirection_node(struct expressionable* expressionable, int ptr_depth, void* right_operand_node_ptr)
{
  struct preprocessor_node* right_operand_node = right_operand_node_ptr;
  void* unary_node = preprocessor_node_create(&(struct preprocessor_node){.type=PREPROCESSOR_UNARY_NODE, .unary_node.op="*", .unary_node.operand_node=right_operand_node, .unary_node.indirection.depth=ptr_depth});
  expressionable_node_push(expressionable, unary_node);
}

void preprocessor_make_expression_node(struct expressionable* expressionable, void* left_node_ptr, void* right_node_ptr, const char* op)
{
  struct preprocessor_node exp_node;
  exp_node.type = PREPROCESSOR_EXPRSSION_NODE;
  exp_node.exp_node.left_node = left_node_ptr;
  exp_node.exp_node.right_node = right_node_ptr;
  exp_node.exp_node.op = op;
  expressionable_node_push(expressionable, preprocessor_node_create(&exp_node));
}

void preprocessor_make_parentheses_node(struct expressionable* expressionable, void* node_ptr)
{
  struct preprocessor_node* node = node_ptr;
  struct preprocessor_node parentheses_node;
  parentheses_node.type = PREPROCESSOR_PARENTHESES_NODE;
  parentheses_node.parenthesis_node.exp = node_ptr;
  expressionable_node_push(expressionable, preprocessor_node_create(&parentheses_node));
}

void preprocessor_make_tenary_node(struct expressionable* expressionable, void* true_result_node_ptr, void* false_result_node_ptr)
{
  struct preprocessor_node* true_result_node = true_result_node_ptr;
  struct preprocessor_node* false_result_node = false_result_node_ptr;
  expressionable_node_push(expressionable, preprocessor_node_create(&(struct preprocessor_node){.type=PREPROCESSOR_TENARY_NODE, .tenary_node.true_node=true_result_node, .tenary_node.false_node=false_result_node}));
}

int preprocessor_get_node_type(struct expressionable* expressionable, void* node)
{
  int generic_type = EXPRESSIONAbLE_GENERIC_TYPE_NON_GENERIC;
  struct preprocessor_node* preprocessor_node = node;
  switch (preprocessor_node->type)
  {
    case PREPROCESSOR_NUMBER_NODE:
      generic_type = EXPRESSIONABLE_GENERIC_TYPE_NUMBER;
      break;

    case PREPROCESSOR_IDENTIFIER_NODE:
    case PREPROCESSOR_KEYWORD_NODE:
      generic_type = EXPRESSIONABLE_GENERIC_TYPE_IDENTIFIER;
      break;

    case PREPROCESSOR_UNARY_NODE:
      generic_type = EXPRESSIONABLE_GENERIC_TYPE_UNARY;
      break;

    case PREPROCESSOR_EXPRSSION_NODE:
      generic_type = EXPRESSIONABLE_GENERIC_TYPE_EXPRESSION;
      break;

    case PREPROCESSOR_PARENTHESES_NODE:
      generic_type = EXPRESSIONABLE_GENERIC_TYPE_PARENTHESES;
      break;
  }

  return generic_type;
}

void* preprocessor_get_left_node(struct expressionable* expressionable, void* target_node)
{
  struct preprocessor_node* node = target_node;
  return node->exp_node.right_node;
}

void* preprocessor_get_right_node(struct expressionable* expressionable, void* target_node)
{
  struct preprocessor_node* node = target_node;
  return node->exp_node.left_node;
}

const char* preprocessor_get_node_operator(struct expressionable* expressionable, void* target_node)
{
  struct preprocessor_node* preprocessor_node = target_node;
  return preprocessor_node->exp_node.op;
}

void** preprocessor_get_left_node_address(struct expressionable* expressionable, void* target_node)
{
  return (void**)&((struct preprocessor_node*)(target_node))->exp_node.left_node;
}

void** preprocessor_get_right_node_address(struct expressionable* expressionable, void* target_node)
{
  return (void**)&((struct preprocessor_node*)(target_node))->exp_node.right_node;
}

void preprocessor_set_expression_node(struct expressionable* expressionable, void* node, void* left_node, void* right_node, const char* op)
{
  struct preprocessor_node* preprocessor_node = node;
  preprocessor_node->exp_node.left_node = left_node;
  preprocessor_node->exp_node.right_node = right_node;
  preprocessor_node->exp_node.op = op;
}

bool preprocessor_should_join_nodes(struct expressionable* expressionable, void* previous_node_ptr, void* node_ptr)
{
  return true;
}

void* preprocessor_join_nodes(struct expressionable* expressionable, void* previous_node_ptr, void* node_ptr)
{
  struct preprocessor_node* previous_node = previous_node_ptr;
  struct preprocessor_node* node = node_ptr;
  return preprocessor_node_create(&(struct preprocessor_node){.type=PREPROCESSOR_JOINED_NODE, .joined_node.left=previous_node, .joined_node.right=node});
}

bool preprocessor_expecting_additional_node(struct expressionable* expressionable, void* node_ptr)
{
  struct preprocessor_node* node = node_ptr;
  return node->type = PREPROCESSOR_KEYWORD_NODE && S_EQ(node->sval, "defined");
}

bool preprocessor_is_custom_operator(struct expressionable* expresisonable, struct token* token)
{
  return false;
}

struct expressionable_config preprocessor_expressionable_config = {
  .callbacks.handle_number = preprocessor_handle_number_token,
  .callbacks.handle_identifier = preprocessor_handle_identifier_token,
  .callbacks.make_unary_node = preprocessor_make_unary_node,
  .callbacks.make_unary_indirection_node = preprocessor_make_unary_indirection_node,
  .callbacks.make_expression_node = preprocessor_make_expression_node,
  .callbacks.make_parentheses_node = preprocessor_make_parentheses_node,
  .callbacks.make_tenary_node = preprocessor_make_tenary_node,
  .callbacks.get_node_type = preprocessor_get_node_type,
  .callbacks.get_left_node = preprocessor_get_left_node,
  .callbacks.get_right_node = preprocessor_get_right_node,
  .callbacks.get_node_operator = preprocessor_get_node_operator,
  .callbacks.get_left_node_address = preprocessor_get_left_node_address,
  .callbacks.get_right_node_address = preprocessor_get_right_node_address,
  .callbacks.set_expression_node = preprocessor_set_expression_node,
  .callbacks.should_join_nodes = preprocessor_should_join_nodes,
  .callbacks.join_nodes = preprocessor_join_nodes,
  .callbacks.expecting_additional_node = preprocessor_expecting_additional_node,
  .callbacks.is_custom_operator = preprocessor_is_custom_operator
};

// > Preprocessor expressionable system end

struct preprocessor_included_file* preprocessor_add_included_file(struct preprocessor* preprocessor, const char* filename)
{
  struct preprocessor_included_file* included_file = calloc(1, sizeof(struct preprocessor_included_file));
  strncpy(included_file->filename, filename, sizeof(included_file->filename));
  vector_push(preprocessor->includes, &included_file);
  return included_file;
}

void preprocessor_create_static_include(struct preprocessor* preprocessor, const char* filename, PREPROCESSOR_STATIC_INCLUDE_HANDLER_POST_CREATION creation_handler)
{
  struct preprocessor_included_file* included_file = preprocessor_add_included_file(preprocessor, filename);
  creation_handler(preprocessor, included_file);
}

struct vector* preprocessor_build_value_vector_for_integer(int value)
{
  struct vector* token_vec = vector_create(sizeof(struct token));
  struct token t1 = {};
  t1.type = TOKEN_TYPE_NUMBER;
  t1.llnum = value;
  vector_push(token_vec, &t1);
  return token_vec;
}

void preprocessor_token_vec_push_keyword_and_identifier(struct vector* token_vec, const char* keyword, const char* identifier)
{
  struct token t1 = {};
  t1.type = TOKEN_TYPE_KEYWORD;
  t1.sval = keyword;
  struct token t2 = {};
  t2.type = TOKEN_TYPE_IDENTIFIER;
  t2.sval = identifier;
  vector_push(token_vec, &t1);
  vector_push(token_vec, &t2);
}

int preprocessor_definition_argument_exists(struct preprocessor_definition* definition, const char* name)
{
  vector_set_peek_pointer(definition->standard.arguments, 0);
  int i = 0;
  const char* current = vector_peek(definition->standard.arguments);
  while (current)
  {
    if (S_EQ(current, name))
      return i;

    i++;
    current = vector_peek(definition->standard.arguments);
  }

  return -1;
}

struct preprocessor_function_argument* preprocessor_function_argument_at(struct preprocessor_function_arguments* arguments, int index)
{
  struct preprocessor_function_argument* argument = vector_at(arguments->arguments, index);
  return argument;
}

void preprocessor_token_push_to_function_arguments(struct preprocessor_function_arguments* arguments, struct token* token)
{
  struct preprocessor_function_argument arg = {};
  arg.tokens = vector_create(sizeof(struct token));
  vector_push(arg.tokens, token);
  vector_push(arguments->arguments, &arg);
}

void preprocessor_function_argument_push_to_vec(struct preprocessor_function_argument* argument, struct vector* vector_out)
{
  vector_set_peek_pointer(argument->tokens, 0);
  struct token* token = vector_peek(argument->tokens);
  while (token)
  {
    vector_push(vector_out, token);
    token = vector_peek(argument->tokens);
  }
}

void preprocessor_handle_token(struct compile_process* compiler, struct token* token)
{
  switch (token->type)
  {
    // Handle all tokens here...
  }
}

int preprocessor_run(struct compile_process* compiler)
{
  #warning "Add our source file as an included file"
  vector_set_peek_pointer(compiler->token_vec_original, 0);
  
  struct token* token = preprocessor_next_token(compiler);
  while (token)
  {
    preprocessor_handle_token(compiler, token);
    token = preprocessor_next_token(compiler);
  }

  return 0;
}