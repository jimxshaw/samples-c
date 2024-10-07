#include <stdio.h>

#define CURRENT_YEAR 2024

int birth_year, age;

int main(void)
{
  printf("Enter your birth year: ");
  scanf("%d", &birth_year);

  if (birth_year % 4 == 0)
  {
    printf("You were born in a leap year!\n");
  }
  else
  {
    printf("You were not born in a leap year.\n");
  }

  age = CURRENT_YEAR - birth_year;

  if (age >= 18)
  {
    printf("You are allowed to vote this year.\n");
  }

  if (age <= 21)
  {
    printf("You are not allowed to drink alcohol.\n");
  }

  return 0;
}
