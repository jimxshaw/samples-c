#include <stdio.h>
#include <stdlib.h>

#define EIGHTYEIGHT 88

int eighty_eight[EIGHTYEIGHT];

int main(void)
{
  for (int i = 0; i < EIGHTYEIGHT; i++)
  {
    eighty_eight[i] = EIGHTYEIGHT;
    printf("eighty_eight[%d] = %d\n", i, eighty_eight[i]);
  }

  return 0;
}