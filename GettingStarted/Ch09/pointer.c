/* Shows basic pointer usage */
#include <stdio.h>

int var = 1;

// Declare a pointer to int.
int *pointer;

int main(void)
{
  // Initialize pointer to point to var.
  pointer = &var;

  // Access variable directly and indirectly.
  printf("direct access, var = %d\n", var);
  printf("indirect access, var = %d\n", *pointer);

  // Memory address of var.
  printf("memory address of var = %p\n", &var);
  printf("memory address of var = %p\n", pointer);

  return 0;
}
