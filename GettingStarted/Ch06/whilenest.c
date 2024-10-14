#include <stdio.h>

int array[5];

int main(void)
{
  int counter = 0;
  int number = 0;

  int arrLen = sizeof(array) / sizeof(array[0]);

  printf("Please input %d numbers\n", arrLen);
  printf("Each number should be from 1 to 10\n");

  while (counter < arrLen)
  {
    number = 0;

    while (number < 1 || number > 10)
    {
      printf("\nInput number %d of %d: ", counter + 1, arrLen);
      scanf("%d", &number);
    }

    array[counter] = number;
    counter++;
  }

  for (counter = 0; counter < arrLen; counter++)
  {
    printf("\nValue %d is %d\n", counter + 1, array[counter]);
  }

  return 0;
}