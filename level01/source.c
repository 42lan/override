#include <stdio.h>

char a_user_name[100];

verify_user_pass()
{
	//0x80486b0:	 "admin"
}

verify_user_name()
{
	// 16 bytes
	puts("verifying username....\n");
	// 0x80486a8 = "dat_wil"
}

int main(void)
{
	// 96 bytes = {0}
	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(a_user_name, 256, stdin);
	if (verify_user_name() != 0)
	{
		puts("nope, incorrect username...\n");
		return(1);
	}
	puts("Enter Password: ");
	fgets(___, 100, stdin);
	if (verify_user_pass() != 0)
	{
		puts("nope, incorrect password...\n");
		return(1);
	}
	return(0);
}
