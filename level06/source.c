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

  nl = strcspn(login, '\n');
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
    //login+3
  }
  return(1);
}

int main(int ac, char **av)
{
  //initial_buffer[80];
  char login[32];      //32 esp+0x2c;
  unsigned int canary; //4
  unsigned int serial; //4

  canary = gs:0x14; //esp+0x4c = gs:0x14 = 0xe6be900; // Save value of canary. It is a random value from gs:0x14
  puts("***********************************");
  puts("*\t\tlevel06\t\t  *");
  puts("***********************************");
  printf("-> Enter Login: ");
  fgets(login, 32, stdin); // $esp+0x2c = 0xffffd6dc = login
  puts("***********************************");
  puts("***** NEW ACCOUNT DETECTED ********");
  puts("***********************************");
  puts("-> Enter Serial: ");
  scanf("%u", &serial); // $esp+0x28 = serial = %esp+0x4
  if (auth(login, serial) == 0)
  {
    puts("Authenticated!");
    system("/bin/sh");
    return(0);
  }
  if((canary ^ gs:0x14) != 0) // :( Check if the canary died (if the value was modified). It is the sign to evacuate from the mine!
    __stack_chk_fail();
  return(1);
}
