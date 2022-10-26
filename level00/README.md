# Hard-coded constant

Log into level00
```shell
[~/42/2022/override]$ ssh 192.168.56.102 -p 4242 -l level00
     ____                  ____  _     __
    / __ \_   _____  _____/ __ \(_)___/ /__
   / / / / | / / _ \/ ___/ /_/ / / __  / _ \
  / /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
  \____/ |___/\___/_/  /_/ |_/_/\__,_/\___/

                       Good luck & Have fun

   To start, ssh with level00/level00 on 192.168.56.102:4242
level00@192.168.56.102's password: level00

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level00/level00
```
An SUID binary is located in the home directory.
```shell
level00@OverRide:~$ ls -l
total 8
-rwsr-s---+ 1 level01 users 7280 Sep 10  2016 level00
```
Only one user function is defined.
```nasm
gdb-peda$ info functions
All defined functions:
[...]
0x08048494  main
[...]
```
It scans input according to a format `%d`, signed decimal integer, compare it to `0x149c` and executes a new shell if equal.
```nasm
gdb-peda$ disassemble main
Dump of assembler code for function main:
   [...]
   0x080484ce <+58>:  mov    eax,0x8048636
   0x080484d3 <+63>:  lea    edx,[esp+0x1c]
   0x080484d7 <+67>:  mov    DWORD PTR [esp+0x4],edx
   0x080484db <+71>:  mov    DWORD PTR [esp],eax
   0x080484de <+74>:  call   0x80483d0 <__isoc99_scanf@plt>
   0x080484e3 <+79>:  mov    eax,DWORD PTR [esp+0x1c]
   0x080484e7 <+83>:  cmp    eax,0x149c
   0x080484ec <+88>:  jne    0x804850d <main+121>
   0x080484ee <+90>:  mov    DWORD PTR [esp],0x8048639
   0x080484f5 <+97>:  call   0x8048390 <puts@plt>
   0x080484fa <+102>: mov    DWORD PTR [esp],0x8048649
   0x08048501 <+109>: call   0x80483a0 <system@plt>
   0x08048506 <+114>: mov    eax,0x0
End of assembler dump.
```
## Exploit
```shell
level00@OverRide:~$ (python -c "print(0x149c)"; echo 'cat /home/users/$(whoami)/.pass') | ./level00
***********************************
*       -Level00 -    *
***********************************
Password:
Authenticated!
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```
