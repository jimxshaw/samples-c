#include <stdio.h>

void print_times_table(int outer, int inner);

main()
{
  int outer = 5;
  int inner = 5;

  printf("The times table:\n");

  print_times_table(outer, inner);

  return;
}

void print_times_table(int outer, int inner)
{
  int a, b;

  for (a = 1; a <= outer; a++)
  {
    for (b = 1; b <= inner; b++)
    {
      printf("%d\t", a * b);
      printf("a:%d, b:%d\t", a, b);
    }

    printf("\n");
  }

  return;
}
