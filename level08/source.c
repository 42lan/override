#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

void log_wrapper(FILE *fd, const char *message, const char *filename)
{
  char dst[300];
  int lenDst;
  unsigned int canary = fs:0x28;

  strcpy(dst, message); //dst = 0x7fffffffe3b0
  lenDst = strlen(dst) - 1;
  snprintf(dst + lenDst, 254 - lenDst, filename);
  dst[strcspn(dst, "\n")] = '\0';
  fprintf(fd, "LOG: %s\n", dst);

  // :( Check if the canary is died (if the value was modified). It is the sign to evacuate from the mine!
  if((canary ^ fs:0x28) != 0)
    __stack_chk_fail();
}

int main(int ac, char **av)
{
  //buffer[176];
  FILE *fd;
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

  file = fopen(av[1], "r");
  if(file == 0)
  path_join = strncat("./backups/", av[1], strlen);
  fd = open(path_join);
  if(fd == 0)
  {
    printf("ERROR: Failed to open %s%s\n", "./backups/", av[1]);
    exit(1);
  }
  fgetc();
  log_wrapper(fd, "Finished back up ", "backups/");
  fclose(fd);
  // :( Check if the canary is died (if the value was modified). It is the sign to evacuate from the mine!
  if((canary ^ fs:0x28) != 0)
    __stack_chk_fail();
  return(0);
}
