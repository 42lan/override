#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int ac, char **av)
{
  //buffer[176];
  int fd;
  unsigned int canary = fs:0x28;

  if(av != 2)
  {
    printf("Usage: %s filename\n", av[0]);
    fd = fopen("./backups/.log", "w");
  }
  if(fd == 0)
  {
    printf("ERROR: Failed to open %s\n", "./backups/.log");
    exit(1);
  }
  log_wrapper(fd, "Starting back up: ", av[1]);

  // :( Check if the canary is died (if the value was modified). It is the sign to evacuate from the mine!
  if((canary ^ fs:0x28) != 0)
    __stack_chk_fail();
  return(0);
}
