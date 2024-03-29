#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clear_stdin(void)
{
  char c = 0;

  do {
    c = getchar();
    if (c == '\n')
      break ;
  } while (c != 0xff);
}

unsigned int get_unum(void)
{
  unsigned int uint = 0;

  fflush(stdout);
  scanf("%u", &uint);
  clear_stdin();
  return uint;
}

void prog_timeout(void)
{
  sys_exit(1);
}

void decrypt(int operand)
{
  int   i;
  char  str[] = "Q}|u`sfg~sf{}|a3\0";
  int   len = strlen(str);

  /*
  ** Knowing key, XOR operation can be easely reversed
  */
  for (i = 0; i < len; i++)
    str[i] = operand ^ str[i];
  if (memcmp(str, "Congratulations!", 17) == 0)
    system("/bin/sh");
  else
    puts("\nInvalid Password");
}

void test(const int password, const int leetdood)
{
  int x;

  x = (leetdood - password) << 2;
  switch (x)
  {
    case 1 ... 21: // Result of x needs to be equal 1 through 21
      decrypt(x);
      break;
    default:
      decrypt(rand());
  }
}

int main(void)
{
  int password;

  srand(time(0));
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  scanf("%d", &password);
  test(password, 0x1337d00d);
  return(0);
}
