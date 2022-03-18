```shell
┌──$ [~/42/2022/override]
└─>  ssh -p4242 192.168.0.23 -l level01
	   ____                  ____  _     __
	  / __ \_   _____  _____/ __ \(_)___/ /__
	 / / / / | / / _ \/ ___/ /_/ / / __  / _ \
	/ /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
	\____/ |___/\___/_/  /_/ |_/_/\__,_/\___/

                       Good luck & Have fun

   To start, ssh with level00/level00 on 192.168.0.23:4242
level01@192.168.0.23's password: uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level01/level01
```
Three user functions and a global uninitialized variable of 100 bytes are defined.
```nasm
gdb-peda$ info functions
All defined functions:

[...]
0x08048464  verify_user_name
0x080484a3  verify_user_pass
0x080484d0  main
[...]
```
```shell
level01@OverRide:~$ objdump -t ./level01 | grep bss
[...]
0804a040 g     O .bss	00000064              a_user_name
```
