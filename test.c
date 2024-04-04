#define TEST_FUNC(s) #s

int printf(const char* s, ...);

int main()
{
  const char* s = TEST_FUNC(hello world);
  printf("%s\n", s);
}