#include <stdio.h>

int get_menu_choice(void);

int main(void)
{
  int choice = get_menu_choice();

  printf("You picked Menu Option %d\n", choice);

  return 0;
}

int get_menu_choice(void)
{
  int selection = 0;

  do
  {
    printf("\n");
    printf("\n1 - Add a Record");
    printf("\n2 - Delete a Record");
    printf("\n3 - Quit");
    printf("\n");
    printf("\nEnter a Selection: ");

    scanf("%d", &selection);

    printf("\n");

  } while (selection < 1 || selection > 3);

  return selection;
}
