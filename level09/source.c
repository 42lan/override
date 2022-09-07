#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void secret_backdoor(void)
{
  char command[128];

  fgets(command, 128, stdin);
  system(command);
}

void set_msg(char *buffer)
{
  char str[128] = {0};

  puts(">: Msg @Unix-Dude");
  printf(">>: ");
  fgets(str, 1024, stdin);
  strncpy(buffer, str, 75);
}

void set_username(char *buffer)
{
  int i = -1;
  char username[128] = {0};

  puts(">: Enter your username");
  printf(">>: ");
  fgets(username, 128, stdin);
  while(++i <= 40 && username[i])
    buffer[i + 140] = username[i];
  printf(">: Welcome, %s", &buffer[140]);
}

void handle_msg(void)
{
  char buffer[180];

  memset(buffer + 140, 0, 40);
  set_username(buffer);
  set_msg(buffer);
  puts(">: Msg sent!");
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
