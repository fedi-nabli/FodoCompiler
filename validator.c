#include "compiler.h"

void validator_initialize(struct compile_process* process)
{
}

void validator_destruct(struct compile_process* process)
{
}

int validate_tree()
{
  return VALIDATOR_ALL_OK;
}

int validate(struct compile_process* process)
{
  int res = 0;
  validator_initialize(process);
  res = validate_tree();
  validator_destruct(process);
  return res;
}