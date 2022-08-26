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

  esp+0x1c = av[1];
  esp+0x18 = env[1];
  canary = gs:0x14; //esp+0x1cc
  [24] = {0};
  [100] = {0};
  [...]
  esp+0x14 = -1;
  //strlen(av[0]);
  memset(av[0], 0, 443);

  // :( Check if the canary died (if the value was modified). It is the sign to evacuate from the mine!
  if((canary ^ gs:0x14) != 0)
    __stack_chk_fail();

}
