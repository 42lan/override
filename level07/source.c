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

int store_number(void)
{
  buffer[40];

  printf(" Index: ");
  index = get_unum(); //ebp-0xc
  return(0);
}

int read_number(void)
{
  printf(" Index: ");
  index = get_unum(); //ebp-0xc
  printf(" Number at data[%u] is %u\n", );
  return(0);
}

int main(int ac, char **av, char **env)
{
  buffer[464];
  int i;
  char buffer[100] = {0};

  esp+0x1c = av[1];
  esp+0x18 = env[1];
  canary = gs:0x14; //esp+0x1cc

  [...]
  memset(av[0], 0, strlen(av[0]));

  i = -1; //esp+0x14
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
    strlen(command);
    if(memcmp(command, "store", 5) == 0)
      store_number(); // store_number(&number);
    else if(memcmp(command, "read", 4) == 0)
    else if(memcmp(command, "quit", 4) == 0)
    else
      printf(" Failed to do %s command\n", command);
  }

  // :( Check if the canary died (if the value was modified). It is the sign to evacuate from the mine!
  if((canary ^ gs:0x14) != 0)
    __stack_chk_fail();

}
