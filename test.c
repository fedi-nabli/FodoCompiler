int printf(const char* s, ...);

int main()
{
  int x;
  x = 0;
  int y;
  int s;
  y = --x;
  s = y++;
  printf("%i\n%i\n", y, s);
}