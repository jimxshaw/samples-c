#include <stdio.h>

#define MAX_COUNT 20

int counter = 1;

int main(void)
{
  while (counter <= MAX_COUNT)
  {
    printf("%d\n", counter);
    counter++;
  }

  return 0;
}
