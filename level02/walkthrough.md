Log into level02
```shell
┌──$ [~/42/2022/override]
└─>  ssh -p 4242 192.168.56.101 -l level02
	   ____                  ____  _     __
	  / __ \_   _____  _____/ __ \(_)___/ /__
	 / / / / | / / _ \/ ___/ /_/ / / __  / _ \
	/ /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
	\____/ |___/\___/_/  /_/ |_/_/\__,_/\___/

                       Good luck & Have fun

   To start, ssh with level00/level00 on 192.168.56.101:4242
level02@192.168.56.101's password: PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level02/level02
```
The binary file located in home directory of `level02` is an 64-bit executable.
```shell
level02@OverRide:~$ ls -l
total 12
-rwsr-s---+ 1 level03 users 9452 Sep 10  2016 level02
level02@OverRide:~$ file level02
level02: setuid setgid ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0xf639d5c443e6ff1c50a0f8393461c0befc329e71, not stripped
```
It contains only one user defined function `main()`




12 times
0x40083d <main+41>:  rep stos QWORD PTR es:[rdi],rax
0x7fffffffe510: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe520: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe530: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe540: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe550: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe560: 0x00000000      0x00000000      0x00000000      0x00000000


0x40085d <main+73>:  rep stos QWORD PTR es:[rdi],rax
0x7fffffffe4e0: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe4f0: 0x00000000      0x00000000      0x00000000      0x00000000
0x7fffffffe500: 0x00000000      0x00000000      0x00000076      0x00000000
