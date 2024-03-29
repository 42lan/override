#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void clear_stdin(void)
{
  char c = 0;

  do {
    c = getchar();
    if (c == '\n')
      break ;
  } while (c != 0xff);
}

unsigned int get_unum(void)
{
  unsigned int uint = 0;

  fflush(stdout);
  scanf("%u", &uint);
  clear_stdin();
  return(uint);
}

void prog_timeout(void)
{
  sys_exit(1);
}

int store_number(char *data)
{
  unsigned int index = 0;
  unsigned int number = 0;

  printf(" Number: ");
  number = get_unum();
  printf(" Index: ");
  index = get_unum();
  if((index % 3) == 0 || index >> 24 == 183)
  {
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    return(1);
  }
  // Determine the absolute address where to store the number
  // Write out of boundaries to overwrite saved EIP in main()
  data[index << 2] = number; // shift is destructive
  return(0);
}

int read_number(const char *data)
{
  unsigned int index;
  unsigned int number;

  printf(" Index: ");
  index = get_unum();
  number = data + (index << 2); // read out of boundaries to see saved EIP in main()
  printf(" Number at data[%u] is %u\n", index, number);
  return(0);
}

int main(int ac, char **av, char **env)
{
  int i;
  int ret;
  unsigned int canary;
  char data[400] = {0};
  char command[20];

  canary = gs:0x14;
  i = -1;
  while(av[++i])
    memset(av[i], 0, strlen(av[i]));
  i = -1;
  while(env[++i])
    memset(env[i], 0, strlen(env[i]));
  puts(
    "----------------------------------------------------\n"
    "  Welcome to wil's crappy number storage service!   \n"
    "----------------------------------------------------\n"
    " Commands:                                          \n"
    "    store - store a number into the data storage    \n"
    "    read  - read a number from the data storage     \n"
    "    quit  - exit the program                        \n"
    "----------------------------------------------------\n"
    "   wil has reserved some storage :>                 \n"
    "----------------------------------------------------\n"
  );
  do {
    printf("Input command: ");
    fgets(command, 20, stdin);
    command[strlen(command) - 1] = '\0';
    if(memcmp(command, "store", 5) == 0)
      ret = store_number(&data);
    else if(memcmp(command, "read", 4) == 0)
      ret = read_number(&data);
    else if(memcmp(command, "quit", 4) == 0)
      break;
    else
      printf(" Failed to do %s command\n", command);

    if(ret == 0)
      printf(" Completed %s command successfully\n", command);
    else
      printf(" Failed to do %s command\n", command);
    memset(command, 0, 20);
  } while(true);

  // :( Check if the canary died (if the value was modified). It is the sign to evacuate from the mine!
  if((canary ^ gs:0x14) != 0)
    __stack_chk_fail();
  return(0);
}
