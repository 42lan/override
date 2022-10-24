Log into `level02`
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
level03@192.168.56.101's password: Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level03/level03
```
Once logged it can be noted that RELRO, STACK CANARY and NX are enabled.

`main()` functon calls `test()` with two arguments, user input based on `%d` conversion specifier and a hard coded value `0x1337d00d`.

`test()` contains switch statement which controls the order of execution/flow by making a decision based on a condition. It defines two condition, a fall through cases from 1 to 21 which calls `decrypt()` with user's input or default case with calling `decrypt()` with random value. The condition is difference between `0x1337d00d` and user input.

`decrypt()` either opens a new process with shell or prints an error message. In order to run a shell, the sting ``"Q}|u`sfg~sf{}|a3\0"`` must be equal to `"Congratulations!"` once it is xored with value of `operand` which is difference between `0x1337d00d` and user input.

To determine which decimal value needs to be entered run Python script:
```python
level03@OverRide:~$ python
Python 2.7.3 (default, Jun 22 2015, 19:33:41)
[GCC 4.6.3] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> for i in range(0x0, 0x15):
...     print("%2d %d" % (i, 0x1337d00d-i))
... 
 0 322424845
 1 322424844
 2 322424843
 3 322424842
 4 322424841
 5 322424840
 6 322424839
 7 322424838
 8 322424837
 9 322424836
10 322424835
11 322424834
12 322424833
13 322424832
14 322424831
15 322424830
16 322424829
17 322424828
18 322424827 *
19 322424826
20 322424825
```

## Exploit
```shell
level03@OverRide:~$ ./level03
***********************************
*               level03         **
***********************************
Password:322424827
$ whoami
level04
$ id
uid=1003(level03) gid=1003(level03) euid=1004(level04) egid=100(users) groups=1004(level04),100(users),1003(level03)
$ cat /home/users/$(whoami)/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```
