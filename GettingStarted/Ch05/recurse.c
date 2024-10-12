/* Recursion demo. Calculates the factorial of a number. */
#include <stdio.h>

int ex;
int result;

unsigned int f, x;

unsigned int factorial(unsigned int a);

int recurse(int n);

int main(void)
{
  puts("Enter an integer >= 0 and <= 8: ");
  scanf("%d", &ex);

  result = recurse(ex);
  printf("result %d\n", result);

  puts("Enter an integer >= 0 and <= 8: ");
  scanf("%d", &x);

  if (x > 8)
  {
    printf("Only values from 0 to 8 are acceptable!\n");
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

int recurse(int n)
{
  if (n == 0)
  {
    return 1;
  }

  return 3 * recurse(n - 1);
}
