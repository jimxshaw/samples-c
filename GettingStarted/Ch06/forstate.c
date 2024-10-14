#include <stdio.h>

#define MAXCOUNT 20

int count;

int main(void)
{
  for (count = 1; count <= MAXCOUNT; count++)
  {
    printf("%d\n", count);
  }

  return 0;
}
