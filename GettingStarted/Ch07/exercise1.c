#include <stdio.h>

#define QUIT 99

int num;
int numbers[6];

void output_array(int numbers[], int size);

int main(void)
{
  int counter = 0;

  do
  {
    puts("Input an even integer please");
    scanf("%d", &num);

    if (num == QUIT)
    {
      break;
    }

    if (counter < 6 && num % 2 == 0)
    {
      numbers[counter] = num;
      counter++;
    }

  } while (num != QUIT && counter < 6);

  printf("Congrats! Here are the numbers: \n");

  output_array(numbers, 6);

  return 0;
}

void output_array(int numbers[], int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("%d\t", numbers[i]);
  }

  printf("\n");
}
