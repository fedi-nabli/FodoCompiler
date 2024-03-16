#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

int main(int argc, const char** argv)
{
  const char* input_file = "./test.c";
  const char* output_file = "./test";
  const char* option = "asm";

  if (argc > 1)
  {
    input_file = argv[1];
  }

  if (argc > 2)
  {
    output_file = argv[2];
  }

  if (argc > 3)
  {
    option = argv[3];
  }

  int compile_flags = COMPILE_PROCESS_OUTPUT_ASM;
  if (S_EQ(option, "exec"))
  {
    compile_flags |= COMPILE_PROCESS_EXECUTE_NASM;
  }
  else if (S_EQ(option, "object"))
  {
    compile_flags |= COMPILE_PROCESS_EXECUTE_NASM | COMPILE_PROCESS_EXPORT_AS_OBJECT;
  }

  int res = compile_file(input_file, output_file, compile_flags);
  if (res == COMPILER_FILE_COMPILED_OK)
  {
    printf("Everything compiled ok\n");
  }
  else if (res == COMPILER_FAILED_WITH_ERRORS)
  {
    printf("Compile failed\n");
  }
  else
  {
    printf("Unknown response for compile time\n");
  }

  if (compile_flags & COMPILE_PROCESS_EXECUTE_NASM)
  {
    char nasm_output_file[40];
    char nasm_cmd[512];
    sprintf(nasm_output_file, "%s.o", output_file);

    if (compile_flags & COMPILE_PROCESS_EXPORT_AS_OBJECT)
    {
      sprintf(nasm_cmd, "nasm -f elf32 %s -o %s", output_file, nasm_output_file);
    }
    else
    {
      sprintf(nasm_cmd, "nasm -f elf32 %s -o %s && gcc -m32 %s -o %s", output_file, nasm_output_file, nasm_output_file, output_file);
    }

    printf("%s\n", nasm_cmd);
    int res = system(nasm_cmd);
    if (res < 0)
    {
      printf("Issue assembling the assembly file with NASM and linking with gcc\n");
      return res;
    }
  }

  return 0;
}