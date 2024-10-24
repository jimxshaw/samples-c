/* Shows the relationship between addresses and elements of */
/* arrays of different data types. */
#include <stdio.h>

int counter;

short arr_short[10];
float arr_float[10];
double arr_double[10];

int main(void)
{
  printf("\t\t\tShort\t\t\tFloat\t\t\tDouble");

  printf("\n===============================================================================");

  // Print the addresses of each array element.
  for (counter = 0; counter < 10; counter++)
  {
    printf("\nElement %d: \t%p\t\t%p\t\t%p", counter, &arr_short[counter], &arr_float[counter], &arr_double[counter]);
  }

  printf("\n===============================================================================\n");

  return 0;
}