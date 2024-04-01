#ifndef FODOCOMPILER_EXPRESSIONABLE_H
#define FODOCOMPILER_EXPRESSIONABLE_H

#include "compiler.h"

#include <stdbool.h>

// < Expressionable enums start
enum
{
  EXPRESSIONABLE_GENERIC_TYPE_NUMBER,
  EXPRESSIONABLE_GENERIC_TYPE_IDENTIFIER,
  EXPRESSIONABLE_GENERIC_TYPE_UNARY,
  EXPRESSIONABLE_GENERIC_TYPE_PARENTHESES,
  EXPRESSIONABLE_GENERIC_TYPE_EXPRESSION,
  EXPRESSIONABLE_GENERIC_TYPE_NON_GENERIC
};

enum
{
  EXPRESSIONABLE_IS_SINGLE,
  EXPRESSIONABLE_IS_PARENTHESES
};

// > Expressionable enums end

// Expressionable flags
enum
{
  EXPRESSIONABLE_FLAG_IS_PREPROCESSOR_EXPRESSION = 0b00000001
};

struct expressionable;

typedef void* (*EXPRESSIONABLE_HANDLE_NUMBER)(struct expressionable* expressionable);
typedef void* (*EXPRESSIONABLE_HANDLE_IDENTIFIER)(struct expressionable* expressionable);
typedef void (*EXPRESSIONABLE_MAKE_EXPRESSION_NODE)(struct expressionable* expressionable, void* left_node_ptr, void* right_node_ptr, const char* op);
typedef void (*EXPRESSIONABLE_MAKE_TENARY_NODE)(struct expressionable* expressionable, void* true_result_node, void* false_result_node);
typedef void (*EXPRESSIONABLE_MAKE_PARENTHESES_NODE)(struct expressionable* expressionable, void* node_ptr);
typedef void (*EXPRESSIONABLE_MAKE_UNARY_NODE)(struct expressionable* expressionable, const char* op, void* right_operand_node_ptr);
typedef void (*EXPRESSIONABLE_MAKE_UNARY_INDIRECTION_NODE)(struct expressionable* expressionable, int ptr_depth, void* right_operand_node_ptr);
typedef int (*EXPRESSIONABLE_GET_NODE_TYPE)(struct expressionable* expressionable, void* node);
typedef void* (*EXPRESSIONABLE_GET_LEFT_NODE)(struct expressionable* expressionable, void* target_node);
typedef void* (*EXPRESSIONABLE_GET_RIGHT_NODE)(struct expressionable* expressionable, void* target_node);
typedef const char* (*EXPRESSIONABLE_GET_NODE_OPERATOR)(struct expressionable* expressionable, void* target_node);
typedef void** (*EXPRESSIONABLE_GET_NODE_ADDRESS)(struct expressionable* expressionable, void* target_node);
typedef void (*EXPRESSIONABLE_SET_EXPRESSION_NODE)(struct expressionable* expressionable, void* node, void* left_node, void* right_node, const char* op);
typedef bool (*EXPRESSIONABLE_SHOULD_JOIN_NODES)(struct expressionable* expressionable, void* previous_node, void* node);
typedef void* (*EXPRESSIONABLE_JOIN_NODES)(struct expressionable* expressionable, void* previous_node, void* node);
typedef bool (*EXPRESSIONABLE_EXPECTING_ADDITIONAL_NODE)(struct expressionable* expressionable, void* node);
typedef bool (*EXPRESSIONABLE_IS_CUSTOM_OPERATOR)(struct expressionable* expressionable, struct token* token);

struct expressionable_config
{
  struct expressionable_callbacks
  {
    // Function pointers
    EXPRESSIONABLE_HANDLE_NUMBER handle_number;
    EXPRESSIONABLE_HANDLE_IDENTIFIER handle_identifier;
    EXPRESSIONABLE_MAKE_EXPRESSION_NODE make_expression_node;
    EXPRESSIONABLE_MAKE_TENARY_NODE make_tenary_node;
    EXPRESSIONABLE_MAKE_PARENTHESES_NODE make_parentheses_node;
    EXPRESSIONABLE_MAKE_UNARY_NODE make_unary_node;
    EXPRESSIONABLE_MAKE_UNARY_INDIRECTION_NODE make_unary_indirection_node;
    EXPRESSIONABLE_GET_NODE_TYPE get_node_type;
    EXPRESSIONABLE_GET_LEFT_NODE get_left_node;
    EXPRESSIONABLE_GET_RIGHT_NODE get_right_node;
    EXPRESSIONABLE_GET_NODE_OPERATOR get_node_operator;
    EXPRESSIONABLE_GET_NODE_ADDRESS get_left_node_address;
    EXPRESSIONABLE_GET_NODE_ADDRESS get_right_node_address;
    EXPRESSIONABLE_SET_EXPRESSION_NODE set_expression_node;
    EXPRESSIONABLE_SHOULD_JOIN_NODES should_join_nodes;
    EXPRESSIONABLE_JOIN_NODES join_nodes;
    EXPRESSIONABLE_EXPECTING_ADDITIONAL_NODE expecting_additional_node;
    EXPRESSIONABLE_IS_CUSTOM_OPERATOR is_custom_operator;
  } callbacks;
};

struct expressionable
{
  int flags;
  struct expressionable_config config;
  struct vector* token_vec;
  struct vector* node_vec_out;
};

// < Expressionable functions start

void expressionable_node_push(struct expressionable* expressionable, void* node_ptr);
void* expressionable_node_pop(struct expressionable* expressionable);
struct token* expressionable_token_next(struct expressionable* expressionable);
void expressionable_parse(struct expressionable* expressionable);
struct expressionable* expressionable_create(struct expressionable_config* config, struct vector* token_vector, struct vector* node_vector, int flags);

// > Expressionable functions end

#endif