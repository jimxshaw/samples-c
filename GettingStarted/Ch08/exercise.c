#include <stdio.h>
#include <stdlib.h>

#define EIGHTYEIGHT 88
#define FIRST 12
#define SECOND 10
#define INITIAL 0

int eighty_eight[EIGHTYEIGHT], stuff[FIRST][SECOND];
int outer, inner;

int main(void)
{
  for (int i = 0; i < EIGHTYEIGHT; i++)
  {
    eighty_eight[i] = EIGHTYEIGHT;
    printf("eighty_eight[%d] = %d\n", i, eighty_eight[i]);
  }

  for (outer = 0; outer < FIRST; outer++)
  {
    for (inner = 0; inner < SECOND; inner++)
    {
      stuff[outer][inner] = INITIAL;
      printf("stuff[%d][%d] = %d\n", outer, inner, stuff[outer][inner]);
    }
  }

  return 0;
}