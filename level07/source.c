#include <stdio.h>
#include <string.h>

void clear_stdin(void)
{
  char c = 0;

  do {
    c = getchar();
    if (c == '\n')
      break ;
  } while (c != 0xff)
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

int store_number(const char *data)
{
  int index = 0;
  int number = 0;

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
  data[index << 2] = number;
  return(0);
}

int read_number(const char *data)
{
  int index;
  int number;

  printf(" Index: ");
  index = get_unum();
  number = data + (index << 2);
  printf(" Number at data[%u] is %u\n", index, number);
  return(0);
}

int main(int ac, char **av, char **env)
{
  buffer[464];
  int i;
  char data[100] = {0};

  canary = gs:0x14; //esp+0x1cc
  i = -1;           //esp+0x14
  while(av[++i])
    memset(av[i], 0, strlen(av[i]));
  i = -1;           //esp+0x14
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
  )
  do {
    printf("Input command: ");
    fgets(command, 20, stdin);
    //strlen(command);
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
  }

  // :( Check if the canary died (if the value was modified). It is the sign to evacuate from the mine!
  if((canary ^ gs:0x14) != 0)
    __stack_chk_fail();

}
