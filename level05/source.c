#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  char str[100];
  int i;

  i = -1;
  fgets(str, 100, stdin);
  while(str[++i])
    if (str[i] >= 'A' || str[i] <= 'Z')
      str[i] ^= ' ';
  printf(str); // FSA to overwrite address of exit() in GOT, ret2shellcode set in ENV
  exit(0);
}
