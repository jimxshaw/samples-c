#include <stdio.h>

int num;
int first_run = 1;

int main(void)
{
  do
  {
    if (first_run != 1)
    {
      puts("Only Even integers are allowed, try again");
    }

    puts("Input an Even integer");
    scanf("%d", &num);

    first_run = 0;

  } while (num % 2 != 0);

  printf("Congrats! Your number is %d\n", num);

  return 0;
}
