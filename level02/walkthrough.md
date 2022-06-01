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

Executable open a `.pass` file of `level03` and read 41 bytes of data from the stream pointed to by the `fd`, storing them at the location given by first pointer to `buff`.
It replace last character `\n` with `\0` and check that letght of buffer is equal to 41.
Then, it invite user to input username and password. So it reads in at most one less than 100 characters from `stdin` and stores them into the buffer pointed to by `username` and `password`. Once input is entered it replace `\n` with `\0`.
Finally, it compares user's password to password read from the file. If it match, it open a new process runnign a shell.
In case if password do not match, it ouputs username and message that it _does not have access!_.
Here `printf()` is ussed is such way that input string is evaluated as a command by the application, which is know as (Format string attack)[https://owasp.org/www-community/attacks/Format_string_attack]. As the password still in the memory, it can be retrieved by passing to `printf()` a conversion specifier `%x`.
