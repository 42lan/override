#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int input; // 32 bytes

	puts("***********************************");
	puts("* \t     -Level00 -\t\t  *");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &input);
	if (input == 0x149c) // The program uses hard-coded constants
	{
		puts("\nAuthenticated!");
		system("/bin/sh");
		return(0);
	}
	puts("\nInvalid Password!");
	return(1);
}
