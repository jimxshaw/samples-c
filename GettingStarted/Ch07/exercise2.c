#include <stdio.h>
#include <stdbool.h>

#define QUIT 99

float a, b;
int array[10];
int minMax[5];

void product(float a, float b);
void sum(int array[], int size);
int findMinMax(int minMax[], int size, bool findMax);

int main(void)
{
  int menuChoice;

  do
  {
    puts("Menu: 1 Product, 2 Sum, 3 MinMax, 99 Quit");
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
      else
      {
        if (menuChoice == 3)
        {
          int counter = 0;
          int input;

          puts("Input a number, or 0 to Quit");

          do
          {
            scanf("%d", &input);

            minMax[counter] = input;
            counter++;

          } while (input != 0 && counter < 5);

          printf("Max: %d\n", findMinMax(minMax, 5, true));
          printf("Max: %d\n", findMinMax(minMax, 5, false));
        }
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

int findMinMax(int minMax[], int size, bool findMax)
{
  int result = minMax[0];

  for (int i = 0; i < size; i++)
  {
    if (findMax)
    {
      if (minMax[i] > result)
      {
        result = minMax[i];
      }
    }
    else
    {
      if (minMax[i] < result)
      {
        result = minMax[i];
      }
    }
  }

  return result;
}
