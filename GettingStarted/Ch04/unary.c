#include <stdio.h>

int a, b;

int main(void)
{
  a = 0;
  b = 0;

  printf("Counting up!\n");
  printf("Post        Pre\n");
  printf("%d          %d\n", a++, ++b);
  printf("%d          %d\n", a++, ++b);
  printf("%d          %d\n", a++, ++b);
  printf("%d          %d\n", a++, ++b);
  printf("%d          %d\n", a++, ++b);

  printf("\nCurrent values of a and b:\n");
  printf("%d          %d\n\n", a, b);

  printf("Counting down!\n");
  printf("Post        Pre\n");
  printf("%d          %d\n", a--, --b);
  printf("%d          %d\n", a--, --b);
  printf("%d          %d\n", a--, --b);
  printf("%d          %d\n", a--, --b);
  printf("%d          %d\n", a--, --b);

  return 0;
}
