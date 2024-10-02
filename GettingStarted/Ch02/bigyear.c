#include <stdio.h>
#define TARGET_AGE 94

int year1, year2;

int calculateYear(int year1);

int main(void)
{
  // Ask for the birth year.
  printf("What year was the person born? ");
  printf("Enter as a 4-digit year (YYYY): ");
  scanf("%d", &year1);

  year2 = calculateYear(year1);

  printf("A person born in %d will be %d in %d\n", year1, TARGET_AGE, year2);

  return 0;
}

int calculateYear(int year1)
{
  return (year1 + TARGET_AGE);
}
