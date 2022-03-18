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
	// 96 bytes in total stack frame ////////////
    char user_pass[64] = {0}; // $esp+0x1c
    int ret = 0;              // $esp+0x5c
    int size;                 // esp+0x4
    int stream;               // esp+0x8

	puts("********* ADMIN LOGIN PROMPT *********");
	printf("Enter Username: ");
	fgets(a_user_name, 256, stdin);
	if ((ret = verify_user_name()) != 0)
	{
		puts("nope, incorrect username...\n");
		return(1);
	}
	puts("Enter Password: ");
	fgets(user_pass, 100, stdin);
	if ((ret = verify_user_pass()) != 0)
	{
		puts("nope, incorrect password...\n");
		return(1);
	}
	return(0);
}
