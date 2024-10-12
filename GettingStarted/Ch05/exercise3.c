#include <stdio.h>

int a, b, result;

int multiply(int a, int b);
int divide(int x, int y);

int main(void)
{
  puts("Input two integers: ");
  scanf("%d %d", &a, &b);

  result = multiply(a, b);

  printf("product: %d\n", result);

  result = divide(a, b);

  if (result == 9999)
  {
    puts("quotient: cannot divide by 0!");
  }
  else
  {
    printf("quotient: %d\n", result);
  }

  return 0;
}

int multiply(int a, int b)
{
  return a * b;
}

int divide(int x, int y)
{
  if (y == 0)
  {
    return 9999;
  }

  return x / y;
}
