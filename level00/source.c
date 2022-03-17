#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int input; // 32 bytes

	puts("***********************************");
	puts("* \t     -Level00 -\t\t  *");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &input); // $esp+0x1c
	if (input == 0x149c)
	{
		puts("\nAuthenticated!");
		system("/bin/sh");
		return(0);
	}
	puts("\nInvalid Password!");
	return(1);
}
