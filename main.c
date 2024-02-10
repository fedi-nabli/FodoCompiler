#include <stdio.h>

#include "helpers/vector.h"

int main()
{
  struct vector* vec = vector_create(sizeof(int));
  int x = 50;
  vector_push(vec, &x);
  x = 20;
  vector_push(vec, &x);

  vector_pop(vec);
  vector_set_peek_pointer(vec, 0);
  int* ptr = vector_peek(vec);
  while (ptr)
  {
    printf("%i\n", *ptr);
    ptr = vector_peek(vec);
  }

  printf("Hello world!\n");
  return 0;
}