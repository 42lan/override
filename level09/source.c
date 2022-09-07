#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void secret_backdoor(void)
{
  char command[128];

  fgets(command, 128, stdin);
  system(command);
}

int main(void)
{
  puts(
    "--------------------------------------------\n"
    "|   ~Welcome to l33t-m$n ~    v1337        |\n"
    "--------------------------------------------\n"
  );
  handle_msg();
  return(0);
}
