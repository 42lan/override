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

void test(const int input, 0x1337d00d)
{
  // BUFFER[40];
  // a = 0x1337d00e - input;
  // ((input << 2) + 134514762)
  switch ()
  {
    case 1 ... 15:
      decrypt(input);
      break ;
    default:
      decrypt(rand());
  }
}

int main(void)
{
	int input;

	srand(time(0));
	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	scanf("%d", input);
	test(input, 0x1337d00d);
	return(0);
}
