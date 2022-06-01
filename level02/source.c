#include <stdio.h>
#include <string.h>

int main()
{
  //stack frame = 288 (0x120)
  char pass[40] = {0};
  char username[] = {0};
  char password[] = {0};
  int len;

  fd = fopen("/home/users/level03/.pass", "r");
  if (fd == 0)
  {
    fwrite("ERROR: failed to open password file\n", 1, 24, stderr);
    exit(1);
  }
  len = fread(pass, 1, 41, fd);
	i = strcspn(pass, "\n");
	pass[i] = '\0';
  if (len != 41)
  {
    fwrite("ERROR: failed to read password file\n", 1, 36, stderr);
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
  i = strcspn(username, "\n"); // return 6 O_o
  username[i] = '\0';

  printf("--[ Password: ");
  fgets(password, 100, stdin);
  i = strcspn(password, "\n"); // return 4
  password[i] = '\0';

  puts("*****************************************");
  if (strncmp(pass, password, 41) != 0)
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
