#include <stdio.h>

int a, b, result;
float af, bf, cf, df, ef, resultf;

int multiply(int a, int b);
int divide(int x, int y);
float average(float af, float bf, float cf, float df, float ef);

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

  puts("Input five floats: ");
  scanf("%f %f %f %f %f", &af, &bf, &cf, &df, &ef);
  resultf = average(af, bf, cf, df, ef);

  printf("average: %f\n", resultf);

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

float average(float a, float b, float c, float d, float e)
{
  return (a + b + c + d + e) / 5;
}
