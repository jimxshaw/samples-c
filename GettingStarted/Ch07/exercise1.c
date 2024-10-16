#include <stdio.h>

#define QUIT 99

int num;
int numbers[6];

int main(void)
{
  int counter = 0;

  do
  {
    puts("Input an integer please");
    scanf("%d", &num);

    if (num == QUIT)
    {
      break;
    }

    if (counter < 6)
    {
      numbers[counter] = num;
      counter++;
    }

  } while (num != QUIT && counter < 6);

  printf("Congrats!\n");

  return 0;
}
