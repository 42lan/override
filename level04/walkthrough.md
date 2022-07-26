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
