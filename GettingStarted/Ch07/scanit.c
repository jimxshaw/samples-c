/* Shows the use of scanf() */
#include <stdio.h>

#define QUIT 4

int get_menu_choice(void);

int main(void)
{
  int choice = 0;
  int int_var = 0;
  float float_var = 0.0;
  unsigned unsigned_var = 0;

  do
  {
    choice = get_menu_choice();

    if (choice == 1)
    {
      puts("Enter a decimal integer: ");
      scanf("%d", &int_var);
    }

    if (choice == 2)
    {
      puts("Enter a decimal floating-point number: ");
      scanf("%f", &float_var);
    }

    if (choice == 3)
    {
      puts("Enter a unsigned decimal number: ");
      scanf("%u", &unsigned_var);
    }

  } while (choice != QUIT);

  printf("\nYour values are: int %d, float %f, unsigned %u \n",
         int_var, float_var, unsigned_var);

  return 0;
}

int get_menu_choice(void)
{
  int selection = 0;
  unsigned char is_first_run = 1;

  do
  {
    if (!is_first_run)
    {
      printf("Invalid selection, please try again or enter 4 to Quit");
    }

    printf("\n");
    printf("\n1 - Get a signed decimal integer");
    printf("\n2 - Get a decimal floating-point number");
    printf("\n3 - Get an unsigned decimal number");
    printf("\n4 - Quit");
    printf("\n");
    printf("\nEnter a Selection: ");

    scanf("%d", &selection);

    printf("\n");

    is_first_run = 0;

  } while (selection < 1 || selection > 4);

  return selection;
}
