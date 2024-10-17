#include <stdio.h>

#define QUIT 99

float a, b;
int array[10];

void product(float a, float b);
void sum(int array[], int size);

int main(void)
{
  int menuChoice;

  do
  {
    puts("Menu: input 1 for Product, 2 for Sum, 99 to Quit");
    scanf("%d", &menuChoice);

    if (menuChoice == 1)
    {
      puts("Product: input 2 float numbers");
      scanf("%f %f", &a, &b);

      product(a, b);
    }
    else
    {
      if (menuChoice == 2)
      {
        int counter = 0;
        int num;

        puts("Product: input 10 numbers");

        do
        {
          scanf("%d", &num);

          array[counter] = num;
          counter++;

        } while (counter < 10);

        sum(array, 10);
      }
    }

  } while (menuChoice != 99);

  return 0;
}

void product(float a, float b)
{
  printf("Product: %f\n", a * b);
}

void sum(int array[], int size)
{
  int result = 0;

  for (int i = 0; i < size; i++)
  {
    result += array[i];
  }

  printf("Sum: %d\n", result);
}
