#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
  char filepass[40] = {0}; // 0x7fffffffe530
  char username[96] = {0}; // 0x7fffffffe560
  char userpass[96] = {0}; // 0x7fffffffe4c0
  int i;
  int len;
  FILE *fd;

  fd = fopen("/home/users/level03/.pass", "r");
  if (fd == 0)
  {
    fwrite("ERROR: failed to open password file\n", 1, 36, stderr);
    exit(1);
  }
  len = fread(filepass, 1, 41, fd);
  i = strcspn(filepass, "\n");
  filepass[i] = '\0';
  if (len != 41)
  {
    fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
    exit(1);
  }
  fclose(fd);
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");

  printf("--[ Username: ");
  fgets(username, 100, stdin);
  i = strcspn(username, "\n");
  username[i] = '\0';

  printf("--[ Password: ");
  fgets(userpass, 100, stdin);
  i = strcspn(userpass, "\n");
  userpass[i] = '\0';

  puts("*****************************************");
  if (strncmp(filepass, userpass, 41) == 0)
  {
    printf("Greetings, %s!\n", username);
    system("/bin/sh");
    return(0);
  }
  else
  {
    printf(username); // format string reading (attack) allow to read content of filepass on stack
    printf(" does not have access!");
    exit(1);
  }
}
