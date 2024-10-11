#include <stdio.h>

int main(void)
{
  int x, y;

  printf("Input x: ");
  scanf("%d", &x);

  printf("Input y: ");
  scanf("%d", &y);

  printf("y before: %d\n", y);

  y = (x >= 1 && x <= 20) ? x : y;

  printf("y after: %d\n", y);

  return 0;
}
