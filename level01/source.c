#include <stdio.h>

char a_user_name[100];

int verify_user_pass(const char *user_pass)
{
    COMPARE user_pass TO "admin"
        return(0); // Valid password
    return(1);     // Invalid password
}

int verify_user_name(void)
{
	puts("verifying username....\n");
    COMPARE a_user_name TO "dat_wil"
        return(0); // Valid username
    return(1);     // Invalid username
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
	if ((ret = verify_user_name()) != 0)
	{
		puts("nope, incorrect username...\n");
		return(1);
	}
	puts("Enter Password: ");
	fgets(user_pass, 100, stdin);
	if ((ret = verify_user_pass(user_pass)) != 0)
	{
		puts("nope, incorrect password...\n");
		return(1);
	}
	return(0);
}
