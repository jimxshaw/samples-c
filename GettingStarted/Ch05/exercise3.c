#include <stdio.h>

int a, b, product;

int multiply(int a, int b);

int main(void)
{
  puts("Input two integers: ");
  scanf("%d %d", &a, &b);

  product = multiply(a, b);

  printf("product: %d\n", product);

  return 0;
}

int multiply(int a, int b)
{
  return a * b;
}
