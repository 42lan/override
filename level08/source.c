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

  strcpy(dst, message);
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
  char c;
  char backup_filename[99];
  unsigned int canary = fs:0x28;
  FILE *stream;
  FILE *fd;

  if(ac != 2)
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
  if ((fd = fopen(av[1], "r")) == 0)
  {
    printf("ERROR: Failed to open %s\n", av[1]);
    exit(1);
  }
  strcpy(backup_filename, "./backups/");
  strncat(backup_filename, av[1], 99 - strlen(backup_filename));

  // 0x0c1 = 193 = 0301 = 00001 | 00100 | 00200 = O_WRONLY | O_CREAT | O_EXCL (/usr/include/asm-generic/fcntl.h)
  // 0x1b0 = 432 = 0660 = 00400 | 00200 | 00040 | 00020 = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP (/usr/include/linux/stat.h)
  stream = open(backup_filename, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  if(stream == 0)
  {
    printf("ERROR: Failed to open %s%s\n", "./backups/", av[1]);
    exit(1);
  }
  while((c = fgetc(stream)) != EOF)
    write(fd, &c, 1);
  log_wrapper(fd, "Finished back up ", av[1]);
  fclose(stream);
  fclose(fd);
  // :( Check if the canary is died (if the value was modified). It is the sign to evacuate from the mine!
  if((canary ^ fs:0x28) != 0)
    __stack_chk_fail();
  return(0);
}
