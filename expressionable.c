#include "compiler.h"
#include "expressionable.h"
#include "helpers/vector.h"

#include <assert.h>

/**
 * @brief Format: {operator1, operator2, operator3, ..., NULL}
 */

struct expressionable_op_precedence_group op_precedence[TOTAL_OPERATOR_GROUPS] = {
  {.operators={"++", "--", "()", "[]", "(", "[", ".", "->", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"*", "/", "%", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"+", "-", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"<<", ">>", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"<", "<=", ">", ">=", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"==", "!=", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"&", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"^", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"|", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"&&", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"||", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT},
  {.operators={"?", ":", NULL}, .associativity=ASSOCIATIVITY_RIGHT_TO_LEFT},
  {.operators={"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", NULL}, .associativity=ASSOCIATIVITY_RIGHT_TO_LEFT},
  {.operators={",", NULL}, .associativity=ASSOCIATIVITY_LEFT_TO_RIGHT}
};

// < Expressionable global functions start

int expressionable_parse_single(struct expressionable* expressionable);

// > Expressionable global functions end

void expressionable_error(struct expressionable* expressionable, const char* str)
{
  assert(0 == 1 && str);
}

struct expressionable_callbacks* expressionable_callbacks(struct expressionable* expressionable)
{
  return &expressionable->config.callbacks;
}

void expressionable_node_push(struct expressionable* expressionable, void* node_ptr)
{
  vector_push(expressionable->node_vec_out, &node_ptr);
}

void* expressionable_node_pop(struct expressionable* expressionable)
{
  void* last_node = vector_back_ptr(expressionable->node_vec_out);
  vector_pop(expressionable->node_vec_out);
  return last_node;
}

void* expressionable_node_peek_or_null(struct expressionable* expressionable)
{
  return vector_back_ptr_or_null(expressionable->node_vec_out);
}

void expressionable_ignore_nl(struct expressionable* expressionable, struct token* next_token)
{
  while (next_token && token_is_symbol(next_token, '\\'))
  {
    // Skip the "\"
    vector_peek(expressionable->token_vec);
    // Skip the new line token
    struct token* nl_token = vector_peek(expressionable->token_vec);
    assert(nl_token->type == TOKEN_TYPE_NEWLINE);
    next_token = vector_peek_no_increment(expressionable->token_vec);
  }
}

struct token* expressionable_peek_next(struct expressionable* expressionable)
{
  struct token* next_token = vector_peek_no_increment(expressionable->token_vec);
  expressionable_ignore_nl(expressionable, next_token);
  return vector_peek_no_increment(expressionable->token_vec);
}

struct token* expressionable_token_next(struct expressionable* expressionable)
{
  struct token* next_token = vector_peek_no_increment(expressionable->token_vec);
  expressionable_ignore_nl(expressionable, next_token);
  return vector_peek(expressionable->token_vec);
}

int expressionable_parse_number(struct expressionable* expressionable)
{
  void* node_ptr = expressionable_callbacks(expressionable)->handle_number(expressionable);
  if (!node_ptr)
  {
    return -1;
  }

  expressionable_node_push(expressionable, node_ptr);
  return 0;
}

int expressionable_parse_identifier(struct expressionable* expressionable)
{
  void* node_ptr = expressionable_callbacks(expressionable)->handle_identifier(expressionable);
  if (!node_ptr)
  {
    return -1;
  }

  expressionable_node_push(expressionable, node_ptr);
  return 0;
}

void expressionable_parse_for_operator(struct expressionable* expressionable)
{
  struct token* op_token = expressionable_peek_next(expressionable);
  const char* op = op_token->sval;
  void* node_left = expressionable_node_peek_or_null(expressionable);
  if (!node_left)
  {
    #warning "Deal with unary"
    return;
  }

  // Pop the operator token
  expressionable_token_next(expressionable);

  // Pop the left node
  expressionable_node_pop(expressionable);

  if (expressionable_peek_next(expressionable)->type == TOKEN_TYPE_OPERATOR)
  {
    if (S_EQ(expressionable_peek_next(expressionable)->sval, "("))
    {
      #warning "Parse for parentheses"
    }
    else if (is_unary_operator(expressionable_peek_next(expressionable)->sval))
    {
      #warning "Parse for unary"
    }
    else
    {
      expressionable_error(expressionable, "The operators are not expected for the given expression");
    }
  }
  else
  {
    // Parse the right node
    expressionable_parse_single(expressionable);
  }

  void* node_right = expressionable_node_pop(expressionable);
  expressionable_callbacks(expressionable)->make_expression_node(expressionable, node_left, node_right, op);
  void* exp_node = expressionable_node_pop(expressionable);
  expressionable_node_push(expressionable, exp_node);
}

int expressionable_parse_exp(struct expressionable* expressionable, struct token* token)
{
  #warning "Parse parentheses"
  #warning "Parse tenary"

  expressionable_parse_for_operator(expressionable);
  return 0;
}

int expressionable_parse_token(struct expressionable* expressionable, struct token* token, int flags)
{
  int res = -1;
  switch (token->type)
  {
    case TOKEN_TYPE_NUMBER:
      res = expressionable_parse_number(expressionable);
      break;

    case NODE_TYPE_IDENTIFIER:
      res = expressionable_parse_identifier(expressionable);
      break;

    case NODE_TYPE_EXPRESSION:
      res = expressionable_parse_exp(expressionable, token);
      break;
  }

  return res;
}

int expressionable_parse_single_with_flags(struct expressionable* expressionable, int flags)
{
  int res = -1;
  struct token* token = expressionable_peek_next(expressionable);
  if (!token)
  {
    return -1;
  }

  if (expressionable_callbacks(expressionable)->is_custom_operator(expressionable, token))
  {
    token->flags |= TOKEN_FLAG_IS_CUSTOM_OPERATOR;
    #warning "Implement expressionable_parse_exp"
  }
  else
  {
    res = expressionable_parse_token(expressionable, token, flags);
  }

  void* node = expressionable_node_pop(expressionable);

  if (expressionable_callbacks(expressionable)->expecting_additional_node(expressionable, node))
  {
    expressionable_parse_single(expressionable);
    void* additional_node = expressionable_node_peek_or_null(expressionable);
    if (expressionable_callbacks(expressionable)->should_join_nodes(expressionable, node, additional_node))
    {
      void* new_node = expressionable_callbacks(expressionable)->join_nodes(expressionable, node, additional_node);
      expressionable_node_pop(expressionable);
      node = new_node;
    }
  }

  expressionable_node_push(expressionable, node);
  return res;
}

int expressionable_parse_single(struct expressionable* expressionable)
{
  return expressionable_parse_single_with_flags(expressionable, 0);
}

void expressionable_parse(struct expressionable* expressionable)
{
  while (expressionable_parse_single(expressionable) == 0)
  {
  }
}