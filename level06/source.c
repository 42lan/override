#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ptrace.h>

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

void enable_timeout_cons(void)
{
  signal(SIGALRM, &prog_timeout);
  alarm(60);
}

auth()
{
}

int main(void)
{
  buffer[80];

  puts("***********************************");
  puts("*\t\tlevel06\t\t  *");
  puts("***********************************");
  printf("-> Enter Login: ");
  fgets(login, 32, stdin);
  puts("***********************************");
  puts("***** NEW ACCOUNT DETECTED ********");
  puts("***********************************");
  puts("-> Enter Serial: ");
  //scanf("%u", 1);
  //if (auth(login, 1) != 0)
  //  return(1);
}
