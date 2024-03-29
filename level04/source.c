#include <unistd.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <linux/prctl.h>

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

void enable_timeout_cons(void)
{
  signal(SIGALRM, &prog_timeout);
  alarm(60);
}

int main(void)
{
  pid_t pid = fork();      //esp+0xac
  char str[128] = {0};     //esp+0x20
  int stat_loc = 0;        //esp+0x1c
  int ret = 0;             //esp+0xa8

  if (pid == 0)
  {
    // Calling process (child) will get SIGHUP when parent dies (it becomes an orphan)
    prctl(PR_SET_PDEATHSIG, SIGHUP);
    // Indicate that the process making this request should be traced
    ptrace(PT_TRACE_ME, NULL, NULL, NULL);
    puts("Give me some shellcode, k");
    gets(str); // enables arbitrarily change a running program's functionality through a buffer overflow attack & ret2lib
  }
  else
  {
    do
    {
      wait(&stat_loc);
      if ((stat_loc & 127) == 0 || (char)((stat_loc & 0x7f) + 1) >> 1)
      {
        puts("child is exiting...");
        return(0);
      }
      ret = ptrace(PTRACE_PEEKUSR, pid, 44, NULL);
    } while (ret != 11); // syscall 0xb (11) = execve
    puts("no exec() for you");
    kill(pid, SIGKILL);
  }
  return(0);
}
