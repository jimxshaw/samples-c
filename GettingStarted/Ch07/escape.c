/* Shows frequently used escape characters */
#include <stdio.h>

#define QUIT 3

int get_menu_choice(void);
void print_report(void);

int main(void)
{
  int choice = 0;

  printf("\"We\'d like to welcome you to the menu program!\"\n");
  printf("Are you ready to make a choice\?\n");

  do
  {
    choice = get_menu_choice();

    if (choice == 1)
    {
      printf("\nBeeping the computer\a\a\a");
    }
    else
    {
      if (choice == 2)
      {
        print_report();
      }
    }
  } while (choice != QUIT);

  printf("You chose to Quit!\n");

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
      printf("Invalid selection, please try again or enter 3 to Quit");
    }

    printf("\n");
    printf("\n1 - Beep the Computer");
    printf("\n2 - Print a Report");
    printf("\n3 - Quit");
    printf("\n");
    printf("\nEnter a Selection: ");

    scanf("%d", &selection);

    printf("\n");

    is_first_run = 0;

  } while (selection < 1 || selection > 3);

  return selection;
}

void print_report(void)
{
  printf("\nSAMPLE REPORT");
  printf("\n\nSequence\tMeaning");
  printf("\n========\t=======");
  printf("\n\\a\t\tbell (alert)");
  printf("\n\\b\t\tbackspace");
  printf("\n...\t\t...");
}
