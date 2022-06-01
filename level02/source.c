#include <stdio.h>
#include <string.h>

int main()
{
  //stack frame = 288 (0x120)
  char buff[40] = {0};
  char username[] = {0};
  char password[] = {0};

  fd = fopen("/home/users/level03/.pass", "r");
  if (fd == 0)
  {
    fwrite("ERROR: failed to open password file\n", 1, 24, stderr);
    exit(1);
  }
  fread(buff, 1, 41, fd);
  if (strcspn(buff, "\n") != 41)
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
  strcspn(username, "\n");
  username[] = "\n";
  printf("--[ Password: ");
  fgets(password, 100, stdin);
  password[] = "\n";
  strcspn(password, "\n");
  puts("*****************************************");
  if (strncmp(, , 41) != 0)
  {
    printf("Greetings, %s!\n", username);
    system("/bin/sh");
    return(0);
  }
  else
  {
    printf(username)
    printf(" does not have access!");
    exit(1);
  }
}
