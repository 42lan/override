#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char str[100]; // esp+0x28
	int i;         // esp+1c

	i = -1;
	fgets(str, 100, stdin);
  while(str[i])
	{
		if (str[i] >= 'A' || str[i] <= 'Z')
			str[i] ^= 0x20;
		i++;
	}
	printf(str);
	exit(0);
}
