/* Shows using pointer arithmetic to access
  array elements with pointer notation. */
#include <stdio.h>

#define MAX 10

int count;

int i_array[MAX] = {0, 1, 2, 3, 4};
float f_array[MAX] = {.0, .1, .2, .3, .4};

int *i_pointer;
float *f_pointer;

// Initialize the pointers.
i_pointer = i_array;
f_pointer = f_array;

main(void)
{
  return 0;
}