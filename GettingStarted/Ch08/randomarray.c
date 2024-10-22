/* Shows multidimensional arrays */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ELEMENTS 1000

int random_array[10][10][10];
int outer, middle, inner;

void outputArray();
void outputMultiArray();
void outputMatchArray();

int main(void)
{
  outputMatchArray();

  outputArray();

  outputMultiArray();

  return 0;
}

void outputArray()
{
  int array[ELEMENTS];
  double sum = 0;
  int count = 1;

  srand(time(NULL));

  for (int i = 0; i < ELEMENTS; i++)
  {
    array[i] = rand();
    sum += array[i];
  }

  printf("Average: %.2f\n", sum / ELEMENTS);

  for (int i = 0; i < ELEMENTS; i++)
  {
    printf("array[%d] = %d\n", i, array[i]);

    if (count == 10)
    {
      printf("\nPress Enter to continue, CTRL+C to quit.\n");
      getchar();

      count = 0;
    }

    count += 1;
  }
}

void outputMultiArray()
{
  srand(time(NULL));

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
}

void outputMatchArray()
{
  int array[10];
  int new[10];

  for (int i = 0; i < 10; i++)
  {
    array[i] = i;
    printf("array[%d] = %d\n", i, array[i]);

    new[i] =
  }
}
