#include <stdio.h>
#include <string.h>

// store dat_wil + shellcode
char a_user_name[100];

int verify_user_pass(const char *user_pass)
{
  if (memcmp(user_pass, "admin", 5) == 0)
    return(0);
  return(1);
}

int verify_user_name(void)
{
  puts("verifying username....\n");
  if (memcmp(a_user_name, "dat_wil", 7) == 0)
    return(0);
  return(1);
}

int main(void)
{
  char user_pass[64] = {0};
  int ret = 0;
  int size;
  int stream;

  puts("********* ADMIN LOGIN PROMPT *********");
  printf("Enter Username: ");
  fgets(a_user_name, 256, stdin); // read more characters than buffer size: store shellcode in a_user_name
  ret = verify_user_name();
  if (ret != 0)
  {
    puts("nope, incorrect username...\n");
    return(1);
  }
  puts("Enter Password: ");
  fgets(user_pass, 100, stdin); // read more character that buffer size: overwrite EIP to ret2shellcode or ret2libc
  ret = verify_user_pass(user_pass);
  if (ret == 0 || ret != 0)
  {
    puts("nope, incorrect password...\n");
    return(1);
  }
  return(0);
}
