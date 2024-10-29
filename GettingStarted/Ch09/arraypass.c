/* Passing an array to a function. */
#include <stdio.h>

#define MAX 5

int array[MAX], count;

int main(void)
{
  for (count = 0; count < MAX; count++)
  {
    printf("\nInput an integer: ");
    scanf("%d", &array[count]);
  }

  printf("\nLargest number in the array: %d\n", largest(array, MAX));

  return 0;
}

int largest(int arr[], int length)
{
  int count;
  int largest = arr[0];

  for (count = 0; count < length; count++)
  {
    if (arr[count] > largest)
    {
      largest = arr[count];
    }
  }

  return largest;
}