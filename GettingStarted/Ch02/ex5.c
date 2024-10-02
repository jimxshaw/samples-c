#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 256

int main(void)
{
  char buffer[BUFF_SIZE];

  printf("Enter your name and press <Enter>:\n");
  fgets(buffer, BUFF_SIZE, stdin);

  printf("\nYour name has %lu characters and spaces!\n", strlen(buffer));

  return 0;
}
