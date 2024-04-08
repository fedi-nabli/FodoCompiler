#include <stdio.h>

int main()
{
  FILE* fp = fopen("./testing.txt", "w");
  fwrite("hello", 5, 1, fp);
}