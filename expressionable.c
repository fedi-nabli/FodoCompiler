#include "compiler.h"

#define TOTAL_OPERATOR_GROUPS 14
#define MAX_OPERATORS_IN_GROUP 12

enum
{
  ASSOCIATIVITY_LEFT_TO_RIGHT,
  ASSOCIATIVITY_RIGHT_TO_LEFT
};

struct expressionable_op_precedence_group
{
  char* operators[MAX_OPERATORS_IN_GROUP];
  int associativity;
};

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