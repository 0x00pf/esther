# ESTHER

ESTHER's The Hardway EscalatoR at least until I come out with a better name :)

There are zillions of enumeration scripts out there intended to support privilege escalation on GNU/Linux systems. Most of them are bash or python scripts that heavily rely on parsing the output of standard tools... 

But what happens if bash or python are not installed in the target machine?

What if some of the used tools does not exists?

I'm thinking on smartphones, routers, or other devices that usually have heavily trimmed down versions of Linux.

So I come up with the idea of coding a native enumeration tool written in C that can be natively run on any device

_Actually, this is just an excuse to learn how all those tools to extract information from a system work_

# Features

Not much yet:

* Dumps environment
* Gets `uname -a` information 
* Dumps `/etc/issue`, `/etc/issue.net`  `/etc/os-release` and `/etc/lsb-release`
* Dumps current user information
* Process `/etc/password`: Shows users with a shell, superusers and current user
* Process `/etc/group`: Shows `adm` and `sudo` group users 
* Search for standard compilers/interpreters
* Search for SUID, SGID, World Writable and World Writable + Executable files
* Dump processes from `/proc`. Shows process name, owner and command-line


# Dependencies

So far no dependency. I hope to keep it like this

# Installation

`./configure && make && sudo make install`

Cross-compilation for ARM or MIPS should be straightforward. Not tested yet

