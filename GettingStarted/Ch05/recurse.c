/* Recursion demo. Calculates the factorial of a number. */
#include <stdio.h>

unsigned int f, x;

unsigned int factorial(unsigned int a);

int main(void)
{
  puts("Enter an integer >= 0 and <= 8: ");
  scanf("%d", &x);

  if (x > 8 || x < 0)
  {
    printf("Only values from 1 to 8 are acceptable!\n");
  }
  else
  {
    f = factorial(x);
    printf("%u factorial equals %u\n", x, f);
  }

  return 0;
}

unsigned int factorial(unsigned int a)
{
  if (a == 1 || a == 0)
  {
    return 1;
  }
  else
  {
    a *= factorial(a - 1);
    return a;
  }
}
