# Format String Attack

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

The issue is that `printf()` is missuded which expose the application to the Format String Attack.

Determine addresses of `system()` and `"/bin/sh"`
```
gdb-peda$ find "/bin/sh"
Searching for '/bin/sh' in: None ranges
Found 1 results, display max 1 items:
libc : 0xf7f897ec ("/bin/sh")
```
