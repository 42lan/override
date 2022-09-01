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

# Exploit
```shell
```
