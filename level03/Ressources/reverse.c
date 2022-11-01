#include <stdio.h>
#include <string.h>

int main(void)
{
  int i;
  int j;
  char str[] = "Q}|u`sfg~sf{}|a3\0";
  int len = strlen(str);

  for (j = 0; j <= 20; j++)
  {
    char  buff[20] = {0};
    for (i = 0; i < len; i++)
      buff[i] = j ^ str[i];
    printf("%2d %d %s\n", j, 0x1337d00d-j, buff);
  }
}
