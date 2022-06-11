#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void clear_stdin(void)
{
  // buffer[24];
  char c = 0;

  do {
    c = getchar();
    if (c == '\n')
      break ;
  } while (c != 0xff)
}

unsigned int get_unum(void)
{
  unsigned int uint = 0;

	fflush(stdout);
	scanf("%u", uint);
	clear_stdin();
	return uint;
}

void prog_timeout(void)
{
  sys_exit(1);
}

void decrypt(int val)
{
	char buff[64]

	"Q}|u`sfg~sf{}|a3\0"

	if ()
		system("/bin/sh");
	else
		puts("\nInvalid Password");
}

test()
{
	char buff[40];

	//if (?? 21)
	//	ja     0x804884a <test+259>
	else
		decrypt(rand())
}

int main(void)
{
	char buff[32];

	srand(time(0));
	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	scanf("%d", buff);
	test(buff, 0x1337d00d);
	return(0);
}
