#include <stdio.h>

int a[5], b[5];

int main(void)
{
  a[0] = 1;
  a[1] = 2;
  a[2] = 3;
  a[3] = 4;
  a[4] = 5;

  for (int i = 0, j = 4; i < 5; i++, j--)
  {
    b[j] = a[i];
  }

  for (int i = 0; i < 5; i++)
  {
    printf("a[%d] = %d\n", i, a[i]);
    printf("b[%d] = %d\n", i, b[i]);
  }

  return 0;
}