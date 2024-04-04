#define TEST(a, b) a ## b

int printf(const char* fmt, ...);

int main()
{
  printf("%i\n", TEST(50, 30));
}