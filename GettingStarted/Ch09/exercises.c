#include <stdio.h>

#define MAX 5

int total;

int arr1[MAX] = {1, 2, 3, 4, 5};
int arr2[MAX] = {2, 4, 6, 8, 10};
int arr3[MAX];

int sumarrays(int array1[], int array2[]);
void addarrays(int array1[], int array2[]);

int main(void)
{
  total = sumarrays(arr1, arr2);

  printf("\nSum of 2 arrays: %d\n", total);

  addarrays(arr1, arr2);

  for (int i = 0; i < MAX; i++)
  {
    printf("\nArray element %d is %d\n", i + 1, arr3[i]);
  }

  return 0;
}

int sumarrays(int array1[], int array2[])
{
  int result = 0;

  for (int i = 0; i < MAX; i++)
  {
    result += array1[i];
    result += array2[i];
  }

  return result;
}

void addarrays(int array1[], int array2[])
{
  for (int i = 0; i < MAX; i++)
  {
    arr3[i] = array1[i] + array2[i];
  }
}
