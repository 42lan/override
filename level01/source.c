#include <stdio.h>
#include <string.h>

char a_user_name[100];

int verify_user_pass(const char *user_pass)
{
  if (memcmp(user_pass, "admin", 5) == 0) // Compare first 5 bytes
    return(0); // Valid password
  return(1); // Invalid password
}

int verify_user_name(void)
{
	puts("verifying username....\n");
  if (memcmp(user_pass, "dat_wil", 7) == 0) // Compare first 7 bytes
    return(0); // Valid username
  return(1); // Invalid username
}

int main(void)
{
  char user_pass[64] = {0}; // $esp+0x1c
  int ret = 0;              // $esp+0x5c
  int size;                 // esp+0x4
  int stream;               // esp+0x8

	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(a_user_name, 256, stdin);
	ret = verify_user_name();
	if (ret != 0)
	{
		puts("nope, incorrect username...\n");
		return(1);
	}
	puts("Enter Password: ");
	fgets(user_pass, 100, stdin);
	ret = verify_user_pass(user_pass);
  //  0x08048589 <+185>:	cmp    DWORD PTR [esp+0x5c],0x0
  //  0x0804858e <+190>:	je     0x8048597 <main+199>
  //  0x08048590 <+192>:	cmp    DWORD PTR [esp+0x5c],0x0
  //  0x08048595 <+197>:	je     0x80485aa <main+218>
	if (ret == 0 || ret != 0)
	{
		puts("nope, incorrect password...\n");
		return(1);
	}
	return(0);
}
