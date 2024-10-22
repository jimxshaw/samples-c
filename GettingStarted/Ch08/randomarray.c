/* Shows multidimensional arrays */
#include <stdio.h>
#include <stdlib.h>

int random_array[10][10][10];
int outer, middle, inner;

int main(void)
{
  for (outer = 0; outer < 10; outer++)
  {
    for (middle = 0; middle < 10; middle++)
    {
      for (inner = 0; inner < 10; inner++)
      {
        random_array[outer][middle][inner] = rand();
      }
    }
  }

  for (outer = 0; outer < 10; outer++)
  {
    for (middle = 0; middle < 10; middle++)
    {
      for (inner = 0; inner < 10; inner++)
      {
        printf("\nrandom_array[%d][%d][%d] = ", outer, middle, inner);
        printf("%d", random_array[outer][middle][inner]);
      }

      printf("\nPress Enter to continue, CTRL+C to quit.");

      getchar();
    }
  }

  return 0;
}
