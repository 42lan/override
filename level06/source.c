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

int auth(char *login, unsigned int serial)
{
  buffer[40];

  nl = strcspn(login, "\n");
  login[nl] = '\0';

  if (strnlen(login, ' ') > 5)
  {
    ret = ptrace(PTRACE_TRACEME, NULL, 1, NULL);
    if(ret == -1)
    {
      puts("\033[32m.---------------------------.");
      puts("\033[31m| !! TAMPERING DETECTED !!  |");
      puts("\033[32m'---------------------------'");
      return(1);
    }
    leetseeded = (login[3] ^ 0x1337) + 0x5eeded;

    i = 0;
    while(i < len)
    {
      if(login[i] <= 0x1f)
        return(1);
      //result += (((login[i] ^ leetseeded) * 0x88233b2b) % 0x539);
      i++;
    }

    if(serial == result)
      return(0);
  }
  return(1);
}

int main(void)
{
  char login[32];
  unsigned int canary;
  unsigned int serial;

  canary = gs:0x14;
  puts("***********************************");
  puts("*\t\tlevel06\t\t  *");
  puts("***********************************");
  printf("-> Enter Login: ");
  fgets(login, 32, stdin);
  puts("***********************************");
  puts("***** NEW ACCOUNT DETECTED ********");
  puts("***********************************");
  puts("-> Enter Serial: ");
  scanf("%u", &serial);
  if (auth(login, serial) == 0)
  {
    puts("Authenticated!");
    system("/bin/sh");
    return(0);
  }
  // :( Check if the canary died (if the value was modified). It is the sign to evacuate from the mine!
  if((canary ^ gs:0x14) != 0)
    __stack_chk_fail();
  return(1);
}
