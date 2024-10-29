/* Shows using pointer arithmetic to access
  array elements with pointer notation. */
#include <stdio.h>

#define MAX 5

int count;

int i_array[MAX] = {0, 1, 2, 3, 4};
float f_array[MAX] = {.0, .1, .2, .3, .4};

int *i_pointer;
float *f_pointer;

main(void)
{
  // Initialize the pointers.
  i_pointer = i_array;
  f_pointer = f_array;

  for (count = 0; count < MAX; count++)
  {
    printf("%d\t%f\n", *i_pointer++, *f_pointer++);
  }

  return 0;
}