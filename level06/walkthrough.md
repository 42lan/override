# Bypass `ptrace` and retrieve serial

```shell
┌──$ [~/42/2022/override]
└─>  ssh 192.168.56.101 -p 4242 -l level06
           ____                  ____  _     __
          / __ \_   _____  _____/ __ \(_)___/ /__
         / / / / | / / _ \/ ___/ /_/ / / __  / _ \
        / /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
        \____/ |___/\___/_/  /_/ |_/_/\__,_/\___/

                       Good luck & Have fun

   To start, ssh with level00/level00 on 192.168.56.101:4242
level06@192.168.56.101's password: h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level06/level06
```
The program gets user input and open a new shell process after authentification based on `auth()` return.

```gdb
level06@OverRide:~$ gdb -q ./level06
Reading symbols from /home/users/level06/level06...(no debugging symbols found)...done.
gdb-peda$ break *auth+78
Breakpoint 1 at 0x8048796
gdb-peda$ start
gdb-peda$ continue
***********************************
*               level06           *
***********************************
-> Enter Login: level06
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 42
```
Computed serial in `auth()` function can be retrived using GDB. It is only a matter of going beyond `ptrace()` by changing `EIP`...
```gdb
gdb-peda$ set $eip=auth+165
gdb-peda$ context
```
... and breaking on instruction where computed serial is compared to user input.
```gdb
gdb-peda$ break *auth+286
Breakpoint 3 at 0x8048866
gdb-peda$ continue
gdb-peda$ x/d $ebp-0x10
0xffffd698:     6233721
gdb-peda$ ^Z
[3]+  Stopped                 gdb ./level06
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: level06
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6233721
Authenticated!
$ 
```

# Exploit
```shell
level06@OverRide:~$ (python -c "print('level06\n' + '6233721\n')"; echo 'id; cat /home/users/$(whoami)/.pass') | ./level06 
***********************************
*               level06           *
***********************************
-> Enter Login: ***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: Authenticated!
uid=1006(level06) gid=1006(level06) euid=1007(level07) egid=100(users) groups=1007(level07),100(users),1006(level06)
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```
