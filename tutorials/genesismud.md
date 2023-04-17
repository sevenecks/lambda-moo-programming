# How to set up a LambdaMOO server on GenesisMud
__By Puff from LambdaMOO__

I'm helping one of the lambdamooers put a MOO up on gensismud.com.

From looking at their site, Genesismud is basically shared hosting for
muds/moos/etc.  Specifically shared hosting, much like 90s vintage
hosting, you're sharing the same linux server as other users.

They don't say it explicitly, but it looks like they don't provide 
anything in the way of handholding for getting your mud up and running.
You get the shell login, and the rest is up to you.

I'm guessing the big difference between genesismud and other ISPs is
that other ISPs would kick you off for running a chat system or mud,
while genesismud is all about that.

Good for them, but I'd think they should offer a little more
help/support, or at least be up front about the lack of help.  Maybe
run some user forums or something.  Not to mention an IRC channel or
mud for their customers to chat about solving technical problems.

They do have some very, very general tutorials on how to install your
mud server, but they're too general to be much use unless you already know
what you're doing, in which case they lack any useful specific data.

For this installation I decided to use the 2008 release of lambdamoo
source, version 1.8.3, from wrog's github account:

https://github.com/wrog/lambdamoo

Click on the "releases" tab, click on the 1.8.3 release link, and
copy the link to download the tar.gz.

When you signed up for genesismud, you got a username and password,
and in the receipt email they sent you a server domain name where your mud
will be hosted.  

Besides your server domain name, username and password, you'll need an
ssh client program.

If you know linux, you already know about ssh.

If you use Windows, I recommend putty.  

If you use a Mac, there should be an ssh client already available via
the shell prompt in your mac.

Run your ssh client, enter the server domain name, and use the username
and password to log into your genesismud shell account.

In your browser, go to the github release download page, copy the link
location for the download, then go back to the ssh window  and use wget
to download the source to your mud account.

```bash
----------------------------------------------------------------------
[snoop@portent ~]$ wget https://github.com/wrog/lambdamoo/archive/v1.8.3.tar.gz
--2015-05-25 13:55:57--  https://github.com/wrog/lambdamoo/archive/v1.8.3.tar.gz
Resolving github.com... 192.30.252.131
Connecting to github.com|192.30.252.131|:443... connected.
HTTP request sent, awaiting response... 302 Found
Location: https://codeload.github.com/wrog/lambdamoo/tar.gz/v1.8.3 [following]
--2015-05-25 13:55:57--  https://codeload.github.com/wrog/lambdamoo/tar.gz/v1.8.3
Resolving codeload.github.com... 192.30.252.146
Connecting to codeload.github.com|192.30.252.146|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: unspecified [application/x-gzip]
Saving to: `v1.8.3'
[snoop@portent ~]$ ls -l
total 762
lrwxrwxrwx   1 snoop snoop     20 May 25 12:20 public_html -> /var/www/users/snoop
-rw-rw-r--   1 snoop snoop 380927 May 25 13:55 v1.8.3
[snoop@portent ~]$ 
----------------------------------------------------------------------
```

For some reason it saved it in a file named v1.8.3 instead of
v1.8.3.tar.gz, so let's rename it, then gunzip it and untar it:

```bash
----------------------------------------------------------------------
[snoop@portent ~]$ mv v1.8.3 v1.8.3.tar.gz
[snoop@portent ~]$ gunzip v1.8.3.tar.gz
[snoop@portent ~]$ ls -l
total 1750
lrwxrwxrwx   1 snoop snoop      20 May 25 12:20 public_html -> /var/www/users/snoop
-rw-rw-r--   1 snoop snoop 1771520 May 25 13:55 v1.8.3.tar
[snoop@portent ~]$ tar -xf v1.8.3.tar
[snoop@portent ~]$ ls -l
total 1743
drwxrwxr-x 3 snoop snoop    5120 Jun 20  2008 lambdamoo-1.8.3
lrwxrwxrwx 1 snoop snoop      20 May 25 12:20 public_html -> /var/www/users/snoop
-rw-rw-r-- 1 snoop snoop 1771520 May 25 13:55 v1.8.3.tar
----------------------------------------------------------------------
```

And now run configure:

```bash
----------------------------------------------------------------------
[snoop@portent ~/]$ cd lambdamoo-1.8.3
[snoop@portent ~/lambdamoo-1.8.3]$ ./configure
checking for bison
checking for gcc
checking how to run the C preprocessor
checking whether -traditional is needed
checking how to run the C preprocessor
checking for NeXT
checking for the DEC Alpha running OSF/1
checking for the SGI compiler
checking for AIX
checking for POSIXized ISC
checking for minix/config.h
checking for -lintl
checking for A/UX
checking for HP/UX
checking that the C compiler handles important ANSI C constructs
checking for a working const keyword
checking how to run the C preprocessor
checking whether stdlib.h declares malloc and friends
checking for sys/bsdtypes.h
checking for sys/select.h
checking for memory.h
checking for pid_t in sys/types.h
checking for mode_t in sys/types.h
checking for size_t in sys/types.h
checking for struct tm in time.h
checking for tm_zone in struct tm
checking for sqrt
checking for sqrt
checking for mkfifo
checking for waitpid
checking for sigemptyset
checking for accept
checking for t_open
checking for t_open
checking for t_open
checking for crypt
checking for crypt
checking for unistd.h
checking for sys/cdefs.h
checking for stdlib.h
checking for tiuser.h
checking for machine/endian.h
checking for remove
checking for rename
checking for poll
checking for select
checking for strerror
checking for strftime
checking for strtoul
checking for matherr
checking for random
checking for lrand48
checking for wait3
checking for wait2
checking for sigsetmask
checking for sigprocmask
checking for sigrelse
checking whether tolower is declared in ctype.h
checking whether fcntl is declared in fcntl.h
checking whether htonl is declared in netinet/in.h
checking whether in_addr_t is declared in netinet/in.h
checking whether ioctl is declared in sys/ioctl.h
checking whether poll is declared in poll.h
checking whether kill is declared in signal.h
checking whether sigemptyset is declared in signal.h
checking whether sigprocmask is declared in signal.h
checking whether sigrelse is declared in signal.h
checking whether accept is declared in sys/socket.h
checking whether bind is declared in sys/socket.h
checking whether shutdown is declared in sys/socket.h
checking whether fstat is declared in sys/stat.h
checking whether fclose is declared in stdio.h
checking whether perror is declared in stdio.h
checking whether remove is declared in stdio.h
checking whether vfprintf is declared in stdio.h
checking whether random is declared in stdlib.h
checking whether srandom is declared in stdlib.h
checking whether strtod is declared in stdlib.h
checking whether strtol is declared in stdlib.h
checking whether strtoul is declared in stdlib.h
checking whether bzero is declared in string.h
checking whether memcpy is declared in string.h
checking whether memset is declared in string.h
checking whether strerror is declared in string.h
checking for bzero being declared in stdlib.h
checking whether getitimer is declared in sys/time.h
checking whether setitimer is declared in sys/time.h
checking whether select is declared in sys/time.h
checking whether strftime is declared in time.h
checking whether time is declared in time.h
checking whether tzname is declared in time.h
checking whether t_open is declared in tiuser.h
checking whether t_errlist is declared in tiuser.h
checking whether fork is declared in unistd.h
checking whether waitpid is declared in sys/wait.h
checking for self-sufficiency of arpa/inet.h
checking for self-sufficiency of signal.h
checking for self-sufficiency of sys/socket.h
checking for self-sufficiency of sys/stat.h
checking for self-sufficiency of time.h
checking for string.h declaring some functions incorrectly
checking for incompatibility between <sys/ioctl.h> and <stropts.h>
checking whether or not fstat() can tell how much data is in a FIFO
checking whether or not select() can be used on FIFOs
checking whether or not poll() can be used on FIFOs
checking whether POSIX-style non-blocking I/O works
checking which MOO networking configurations are likely to work...
checking for sys/socket.h
checking for telnet
----------------------------------------------------------------------
| The following networking configurations will probably work on your
| system; any configuration *not* listed here almost certainly will
| *not* work on your system:
|
|   NP_SINGLE NS_BSD/NP_LOCAL NS_BSD/NP_TCP NS_SYSV/NP_LOCAL
----------------------------------------------------------------------
creating config.status
creating Makefile
creating config.h
[snoop@portent ~/lambdamoo-1.8.3]$
----------------------------------------------------------------------
```

Configure ran a script that found a lot of tools and etc on the
system, and went in and modified the make file and other files so they
would know where the tools are.

And now it's time to run make:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ make
gcc -g  -O  -c -o ast.o ast.c
gcc -g  -O  -c -o code_gen.o code_gen.c
gcc -g  -O  -c -o db_file.o db_file.c
gcc -g  -O  -c -o db_io.o db_io.c
gcc -g  -O  -c -o db_objects.o db_objects.c
gcc -g  -O  -c -o db_properties.o db_properties.c
gcc -g  -O  -c -o db_verbs.o db_verbs.c
gcc -g  -O  -c -o decompile.o decompile.c
gcc -g  -O  -c -o disassemble.o disassemble.c
gcc -g  -O  -c -o eval_env.o eval_env.c
gcc -g  -O  -c -o eval_vm.o eval_vm.c
gcc -g  -O  -c -o exceptions.o exceptions.c
gcc -g  -O  -c -o execute.o execute.c
gcc -g  -O  -c -o extensions.o extensions.c
gcc -g  -O  -c -o functions.o functions.c
bison -y -d parser.y
mv -f y.tab.c parser.c
gcc -g  -O  -c -o parser.o parser.c
touch y.tab.h
gcc -g  -O  -c -o keywords.o keywords.c
gcc -g  -O  -c -o list.o list.c
gcc -g  -O  -c -o log.o log.c
gcc -g  -O  -c -o malloc.o malloc.c
gcc -g  -O  -c -o match.o match.c
gcc -g  -O  -c -o md5.o md5.c
gcc -g  -O  -c -o name_lookup.o name_lookup.c
touch net_single.o
touch net_multi.o
gcc -g  -O  -c -o network.o network.c
touch net_mp_selct.o
touch net_mp_poll.o
touch net_mp_fake.o
gcc -g  -O  -c -o net_mplex.o net_mplex.c
touch net_bsd_tcp.o
touch net_bsd_lcl.o
touch net_sysv_tcp.o
touch net_sysv_lcl.o
gcc -g  -O  -c -o net_proto.o net_proto.c
gcc -g  -O  -c -o numbers.o numbers.c
gcc -g  -O  -c -o objects.o objects.c
gcc -g  -O  -c -o parse_cmd.o parse_cmd.c
gcc -g  -O  -c -o pattern.o pattern.c
gcc -g  -O  -c -o program.o program.c
gcc -g  -O  -c -o property.o property.c
gcc -g  -O  -c -o quota.o quota.c
gcc -g  -O  -c -o ref_count.o ref_count.c
gcc -g  -O  -c -o regexpr.o regexpr.c
gcc -g  -O  -c -o server.o server.c
gcc -g  -O  -c -o storage.o storage.c
gcc -g  -O  -c -o streams.o streams.c
gcc -g  -O  -c -o str_intern.o str_intern.c
gcc -g  -O  -c -o sym_table.o sym_table.c
gcc -g  -O  -c -o tasks.o tasks.c
gcc -g  -O  -c -o timers.o timers.c
gcc -g  -O  -c -o unparse.o unparse.c
gcc -g  -O  -c -o utils.o utils.c
gcc -g  -O  -c -o verbs.o verbs.c
gcc -g  -O  -c -o version.o version.c
gcc -g  -O ast.o code_gen.o db_file.o db_io.o db_objects.o db_properties.o db_verbs.o decompile.o disassemble.o eval_env.o eval_vm.o exceptions.o execute.o extensions.o
functions.o keywords.o list.o log.o malloc.o match.o md5.o name_lookup.o network.o net_mplex.o net_proto.o numbers.o objects.o parse_cmd.o pattern.o program.o property.o
quota.o ref_count.o regexpr.o server.o storage.o streams.o str_intern.o sym_table.o tasks.o timers.o unparse.o utils.o verbs.o version.o parser.o -lm -lcrypt -o moo
rm parser.c
[snoop@portent ~/lambdamoo-1.8.3]$
----------------------------------------------------------------------
```

Yay, no errors!

You will note that the directory now contains some extra files:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ ls -l
total 3468
-rw-rw-r-- 1 snoop snoop   1287 Jun 20  2008 aclocal.m4
-rw-rw-r-- 1 snoop snoop   2234 Jun 20  2008 AddingNewMOOTypes.txt
-rw-rw-r-- 1 snoop snoop  10326 Jun 20  2008 ast.c
-rw-rw-r-- 1 snoop snoop   6388 Jun 20  2008 ast.h
-rw-rw-r-- 1 snoop snoop  20120 May 25 14:14 ast.o
-rw-rw-r-- 1 snoop snoop   2211 Jun 20  2008 bf_register.h
-rw-rw-r-- 1 snoop snoop 145516 Jun 20  2008 ChangeLog.txt
-rw-rw-r-- 1 snoop snoop   3304 Jun 20  2008 client_bsd.c
-rw-rw-r-- 1 snoop snoop   5911 Jun 20  2008 client_sysv.c
-rw-rw-r-- 1 snoop snoop  37677 Jun 20  2008 code_gen.c
-rw-rw-r-- 1 snoop snoop   1660 Jun 20  2008 code_gen.h
-rw-rw-r-- 1 snoop snoop  41064 May 25 14:14 code_gen.o
-rw-rw-r-- 1 snoop snoop  13184 May 25 14:11 config.h
-rw-rw-r-- 1 snoop snoop  12684 Jun 20  2008 config.h.in
-rwxrwxr-x 1 snoop snoop   9385 May 25 14:11 config.status
-rwxrwxr-x 1 snoop snoop  75850 Jun 20  2008 configure
-rw-rw-r-- 1 snoop snoop  19518 Jun 20  2008 configure.in
-rw-rw-r-- 1 snoop snoop  19764 Jun 20  2008 db_file.c
-rw-rw-r-- 1 snoop snoop  35352 May 25 14:14 db_file.o
-rw-rw-r-- 1 snoop snoop  19878 Jun 20  2008 db.h
-rw-rw-r-- 1 snoop snoop  10256 Jun 20  2008 db_io.c
-rw-rw-r-- 1 snoop snoop   4571 Jun 20  2008 db_io.h
-rw-rw-r-- 1 snoop snoop  20200 May 25 14:14 db_io.o
-rw-rw-r-- 1 snoop snoop  13499 Jun 20  2008 db_objects.c
-rw-rw-r-- 1 snoop snoop  25568 May 25 14:14 db_objects.o
-rw-rw-r-- 1 snoop snoop   5170 Jun 20  2008 db_private.h
-rw-rw-r-- 1 snoop snoop  16381 Jun 20  2008 db_properties.c
-rw-rw-r-- 1 snoop snoop  29992 May 25 14:14 db_properties.o
-rw-rw-r-- 1 snoop snoop    186 Jun 20  2008 db_tune.h
-rw-rw-r-- 1 snoop snoop  19085 Jun 20  2008 db_verbs.c
-rw-rw-r-- 1 snoop snoop  30052 May 25 14:14 db_verbs.o
-rw-rw-r-- 1 snoop snoop  28968 Jun 20  2008 decompile.c
-rw-rw-r-- 1 snoop snoop   1655 Jun 20  2008 decompile.h
-rw-rw-r-- 1 snoop snoop  42304 May 25 14:14 decompile.o
-rw-rw-r-- 1 snoop snoop  14054 Jun 20  2008 disassemble.c
-rw-rw-r-- 1 snoop snoop   1679 Jun 20  2008 disassemble.h
-rw-rw-r-- 1 snoop snoop  24908 May 25 14:14 disassemble.o
-rw-rw-r-- 1 snoop snoop   4776 Jun 20  2008 eval_env.c
-rw-rw-r-- 1 snoop snoop   2292 Jun 20  2008 eval_env.h
-rw-rw-r-- 1 snoop snoop  10120 May 25 14:14 eval_env.o
-rw-rw-r-- 1 snoop snoop   4975 Jun 20  2008 eval_vm.c
-rw-rw-r-- 1 snoop snoop   2355 Jun 20  2008 eval_vm.h
-rw-rw-r-- 1 snoop snoop  10908 May 25 14:14 eval_vm.o
-rw-rw-r-- 1 snoop snoop   3562 Jun 20  2008 exceptions.c
-rw-rw-r-- 1 snoop snoop   6488 Jun 20  2008 exceptions.h
-rw-rw-r-- 1 snoop snoop   4104 May 25 14:14 exceptions.o
-rw-rw-r-- 1 snoop snoop  83950 Jun 20  2008 execute.c
-rw-rw-r-- 1 snoop snoop   7775 Jun 20  2008 execute.h
-rw-rw-r-- 1 snoop snoop 137052 May 25 14:14 execute.o
-rw-rw-r-- 1 snoop snoop   5672 Jun 20  2008 extensions.c
-rw-rw-r-- 1 snoop snoop   9788 May 25 14:14 extensions.o
-rw-rw-r-- 1 snoop snoop  15741 Jun 20  2008 functions.c
-rw-rw-r-- 1 snoop snoop   5092 Jun 20  2008 functions.h
-rw-rw-r-- 1 snoop snoop  24928 May 25 14:14 functions.o
-rw-rw-r-- 1 snoop snoop   1889 Jun 20  2008 getpagesize.h
-rw-rw-r-- 1 snoop snoop  26564 Jun 20  2008 gnu-malloc.c
-rw-rw-r-- 1 snoop snoop   5922 Jun 20  2008 keywords.c
-rw-rw-r-- 1 snoop snoop   3034 Jun 20  2008 keywords.gperf
-rw-rw-r-- 1 snoop snoop   2063 Jun 20  2008 keywords.h
-rw-rw-r-- 1 snoop snoop  10052 May 25 14:14 keywords.o
-rw-rw-r-- 1 snoop snoop  31730 Jun 20  2008 list.c
-rw-rw-r-- 1 snoop snoop   2452 Jun 20  2008 list.h
-rw-rw-r-- 1 snoop snoop  57384 May 25 14:14 list.o
-rw-rw-r-- 1 snoop snoop   5632 Jun 20  2008 log.c
-rw-rw-r-- 1 snoop snoop   2536 Jun 20  2008 log.h
-rw-rw-r-- 1 snoop snoop  13044 May 25 14:14 log.o
-rw-rw-r-- 1 snoop snoop  22327 May 25 14:11 Makefile
-rw-rw-r-- 1 snoop snoop  22259 Jun 20  2008 Makefile.in
-rw-rw-r-- 1 snoop snoop   2877 Jun 20  2008 malloc.c
-rw-rw-r-- 1 snoop snoop   1684 May 25 14:14 malloc.o
-rw-rw-r-- 1 snoop snoop   4819 Jun 20  2008 match.c
-rw-rw-r-- 1 snoop snoop   1976 Jun 20  2008 match.h
-rw-rw-r-- 1 snoop snoop   9036 May 25 14:14 match.o
-rw-rw-r-- 1 snoop snoop  10682 Jun 20  2008 md5.c
-rw-rw-r-- 1 snoop snoop   2784 Jun 20  2008 md5.h
-rw-rw-r-- 1 snoop snoop  10092 May 25 14:14 md5.o
-rw-rw-r-- 1 snoop snoop    321 Jun 20  2008 Minimal.db
-rwxrwxr-x 1 snoop snoop 778881 May 25 14:14 moo
-rw-rw-r-- 1 snoop snoop   5573 Jun 20  2008 MOOCodeSequences.txt
-rw-rw-r-- 1 snoop snoop   1605 Jun 20  2008 my-ctype.h
-rw-rw-r-- 1 snoop snoop   2325 Jun 20  2008 my-fcntl.h
-rw-rw-r-- 1 snoop snoop   1788 Jun 20  2008 my-inet.h
-rw-rw-r-- 1 snoop snoop   2919 Jun 20  2008 my-in.h
-rw-rw-r-- 1 snoop snoop   2034 Jun 20  2008 my-ioctl.h
-rw-rw-r-- 1 snoop snoop   1789 Jun 20  2008 my-math.h
-rw-rw-r-- 1 snoop snoop   1742 Jun 20  2008 my-poll.h
-rw-rw-r-- 1 snoop snoop   2713 Jun 20  2008 my-signal.h
-rw-rw-r-- 1 snoop snoop   3006 Jun 20  2008 my-socket.h
-rw-rw-r-- 1 snoop snoop   2211 Jun 20  2008 my-stat.h
-rw-rw-r-- 1 snoop snoop   2722 Jun 20  2008 my-stdarg.h
-rw-rw-r-- 1 snoop snoop   3414 Jun 20  2008 my-stdio.h
-rw-rw-r-- 1 snoop snoop   3422 Jun 20  2008 my-stdlib.h
-rw-rw-r-- 1 snoop snoop   4079 Jun 20  2008 my-string.h
-rw-rw-r-- 1 snoop snoop   1613 Jun 20  2008 my-stropts.h
-rw-rw-r-- 1 snoop snoop   2547 Jun 20  2008 my-sys-time.h
-rw-rw-r-- 1 snoop snoop   2660 Jun 20  2008 my-time.h
-rw-rw-r-- 1 snoop snoop   2284 Jun 20  2008 my-tiuser.h
-rw-rw-r-- 1 snoop snoop   3294 Jun 20  2008 my-types.h
-rw-rw-r-- 1 snoop snoop   3235 Jun 20  2008 my-unistd.h
-rw-rw-r-- 1 snoop snoop   1713 Jun 20  2008 my-wait.h
-rw-rw-r-- 1 snoop snoop  12033 Jun 20  2008 name_lookup.c
-rw-rw-r-- 1 snoop snoop   2380 Jun 20  2008 name_lookup.h
-rw-rw-r-- 1 snoop snoop  15912 May 25 14:14 name_lookup.o
-rw-rw-r-- 1 snoop snoop   5649 Jun 20  2008 net_bsd_lcl.c
-rw-rw-r-- 1 snoop snoop      0 May 25 14:14 net_bsd_lcl.o
-rw-rw-r-- 1 snoop snoop  10971 Jun 20  2008 net_bsd_tcp.c
-rw-rw-r-- 1 snoop snoop      0 May 25 14:14 net_bsd_tcp.o
-rw-rw-r-- 1 snoop snoop   4603 Jun 20  2008 net_mp_fake.c
-rw-rw-r-- 1 snoop snoop      0 May 25 14:14 net_mp_fake.o
-rw-rw-r-- 1 snoop snoop   1680 Jun 20  2008 net_mplex.c
-rw-rw-r-- 1 snoop snoop   3424 Jun 20  2008 net_mplex.h
-rw-rw-r-- 1 snoop snoop   7432 May 25 14:14 net_mplex.o
-rw-rw-r-- 1 snoop snoop   2948 Jun 20  2008 net_mp_poll.c
-rw-rw-r-- 1 snoop snoop      0 May 25 14:14 net_mp_poll.o
-rw-rw-r-- 1 snoop snoop   2885 Jun 20  2008 net_mp_selct.c
-rw-rw-r-- 1 snoop snoop      0 May 25 14:14 net_mp_selct.o
-rw-rw-r-- 1 snoop snoop  20641 Jun 20  2008 net_multi.c
-rw-rw-r-- 1 snoop snoop   2407 Jun 20  2008 net_multi.h
-rw-rw-r-- 1 snoop snoop      0 May 25 14:14 net_multi.o
-rw-rw-r-- 1 snoop snoop   1861 Jun 20  2008 net_proto.c
-rw-rw-r-- 1 snoop snoop   7564 Jun 20  2008 net_proto.h
-rw-rw-r-- 1 snoop snoop  14500 May 25 14:14 net_proto.o
-rw-rw-r-- 1 snoop snoop   6918 Jun 20  2008 net_single.c
-rw-rw-r-- 1 snoop snoop      0 May 25 14:14 net_single.o
-rw-rw-r-- 1 snoop snoop  12664 Jun 20  2008 net_sysv_lcl.c
-rw-rw-r-- 1 snoop snoop      0 May 25 14:14 net_sysv_lcl.o
-rw-rw-r-- 1 snoop snoop  12745 Jun 20  2008 net_sysv_tcp.c
-rw-rw-r-- 1 snoop snoop      0 May 25 14:14 net_sysv_tcp.o
-rw-rw-r-- 1 snoop snoop   2186 Jun 20  2008 net_tcp.c
-rw-rw-r-- 1 snoop snoop   2528 Jun 20  2008 network.c
-rw-rw-r-- 1 snoop snoop  10961 Jun 20  2008 network.h
-rw-rw-r-- 1 snoop snoop  28640 May 25 14:14 network.o
-rw-rw-r-- 1 snoop snoop  18243 Jun 20  2008 numbers.c
-rw-rw-r-- 1 snoop snoop   1674 Jun 20  2008 numbers.h
-rw-rw-r-- 1 snoop snoop  44504 May 25 14:14 numbers.o
-rw-rw-r-- 1 snoop snoop  17226 Jun 20  2008 objects.c
-rw-rw-r-- 1 snoop snoop  32104 May 25 14:14 objects.o
-rw-rw-r-- 1 snoop snoop   5939 Jun 20  2008 opcode.h
-rw-rw-r-- 1 snoop snoop  21644 Jun 20  2008 options.h
-rw-rw-r-- 1 snoop snoop   8040 Jun 20  2008 parse_cmd.c
-rw-rw-r-- 1 snoop snoop   2863 Jun 20  2008 parse_cmd.h
-rw-rw-r-- 1 snoop snoop  12904 May 25 14:14 parse_cmd.o
-rw-rw-r-- 1 snoop snoop   2478 Jun 20  2008 parser.h
-rw-rw-r-- 1 snoop snoop  50772 May 25 14:14 parser.o
-rw-rw-r-- 1 snoop snoop  30392 Jun 20  2008 parser.y
-rw-rw-r-- 1 snoop snoop   4874 Jun 20  2008 pattern.c
-rw-rw-r-- 1 snoop snoop   1901 Jun 20  2008 pattern.h
-rw-rw-r-- 1 snoop snoop   9572 May 25 14:14 pattern.o
drwxrwxr-x 2 snoop snoop   1024 Jun 20  2008 pgperf
-rw-rw-r-- 1 snoop snoop   5040 Jun 20  2008 program.c
-rw-rw-r-- 1 snoop snoop   3249 Jun 20  2008 program.h
-rw-rw-r-- 1 snoop snoop   9576 May 25 14:14 program.o
-rw-rw-r-- 1 snoop snoop  10588 Jun 20  2008 property.c
-rw-rw-r-- 1 snoop snoop  20376 May 25 14:14 property.o
-rw-rw-r-- 1 snoop snoop   2749 Jun 20  2008 quota.c
-rw-rw-r-- 1 snoop snoop   1681 Jun 20  2008 quota.h
-rw-rw-r-- 1 snoop snoop   6876 May 25 14:14 quota.o
-rw-rw-r-- 1 snoop snoop   2526 Jun 20  2008 random.h
-rw-rw-r-- 1 snoop snoop  10655 Jun 20  2008 README
-rw-rw-r-- 1 snoop snoop   2907 Jun 20  2008 README.Minimal
-rw-rw-r-- 1 snoop snoop  10121 Jun 20  2008 README.rX
-rw-rw-r-- 1 snoop snoop   6144 Jun 20  2008 ref_count.c
-rw-rw-r-- 1 snoop snoop   2006 Jun 20  2008 ref_count.h
-rw-rw-r-- 1 snoop snoop   3500 May 25 14:14 ref_count.o
-rw-rw-r-- 1 snoop snoop  43328 Jun 20  2008 regexpr.c
-rw-rw-r-- 1 snoop snoop   5368 Jun 20  2008 regexpr.h
-rw-rw-r-- 1 snoop snoop  39748 May 25 14:14 regexpr.o
-rwxrwxr-x 1 snoop snoop   2066 Jun 20  2008 restart
-rwxrwxr-x 1 snoop snoop   1808 Jun 20  2008 restart.sh
-rw-rw-r-- 1 snoop snoop  52071 Jun 20  2008 server.c
-rw-rw-r-- 1 snoop snoop   9420 Jun 20  2008 server.h
-rw-rw-r-- 1 snoop snoop  74192 May 25 14:14 server.o
-rw-rw-r-- 1 snoop snoop   8737 Jun 20  2008 storage.c
-rw-rw-r-- 1 snoop snoop   4989 Jun 20  2008 storage.h
-rw-rw-r-- 1 snoop snoop   9852 May 25 14:14 storage.o
-rw-rw-r-- 1 snoop snoop   6490 Jun 20  2008 streams.c
-rw-rw-r-- 1 snoop snoop   2374 Jun 20  2008 streams.h
-rw-rw-r-- 1 snoop snoop  11936 May 25 14:14 streams.o
-rw-rw-r-- 1 snoop snoop   6347 Jun 20  2008 str_intern.c
-rw-rw-r-- 1 snoop snoop    757 Jun 20  2008 str_intern.h
-rw-rw-r-- 1 snoop snoop  10616 May 25 14:14 str_intern.o
-rw-rw-r-- 1 snoop snoop   6613 Jun 20  2008 structures.h
-rw-rw-r-- 1 snoop snoop   5633 Jun 20  2008 sym_table.c
-rw-rw-r-- 1 snoop snoop   2927 Jun 20  2008 sym_table.h
-rw-rw-r-- 1 snoop snoop   9320 May 25 14:14 sym_table.o
-rw-rw-r-- 1 snoop snoop  63703 Jun 20  2008 tasks.c
-rw-rw-r-- 1 snoop snoop   6760 Jun 20  2008 tasks.h
-rw-rw-r-- 1 snoop snoop  75804 May 25 14:14 tasks.o
-rw-rw-r-- 1 snoop snoop   7881 Jun 20  2008 timers.c
-rw-rw-r-- 1 snoop snoop   2214 Jun 20  2008 timers.h
-rw-rw-r-- 1 snoop snoop  10180 May 25 14:14 timers.o
-rw-rw-r-- 1 snoop snoop   1387 Jun 20  2008 tokens.h
-rw-rw-r-- 1 snoop snoop  21078 Jun 20  2008 unparse.c
-rw-rw-r-- 1 snoop snoop   2816 Jun 20  2008 unparse.h
-rw-rw-r-- 1 snoop snoop  27672 May 25 14:14 unparse.o
-rw-rw-r-- 1 snoop snoop  14348 Jun 20  2008 utils.c
-rw-rw-r-- 1 snoop snoop   5102 Jun 20  2008 utils.h
-rw-rw-r-- 1 snoop snoop  19976 May 25 14:14 utils.o
-rw-rw-r-- 1 snoop snoop  17857 Jun 20  2008 verbs.c
-rw-rw-r-- 1 snoop snoop   1605 Jun 20  2008 verbs.h
-rw-rw-r-- 1 snoop snoop  31668 May 25 14:14 verbs.o
-rw-rw-r-- 1 snoop snoop   2016 Jun 20  2008 version.c
-rw-rw-r-- 1 snoop snoop   3478 Jun 20  2008 version.h
-rw-rw-r-- 1 snoop snoop   2840 May 25 14:14 version.o
-rw-rw-r-- 1 snoop snoop   3642 May 25 14:14 y.tab.h
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

You'll notice a lot of files named something.o.  Those are the chunks
that the compiler generates, which are then all linked together to
produce the file named "moo".  Leaving aside the .o files, we have the
following new files:

```bash
----------------------------------------------------------------------
-rw-rw-r-- 1 snoop snoop  13184 May 25 14:11 config.h
-rwxrwxr-x 1 snoop snoop   9385 May 25 14:11 config.status
-rw-rw-r-- 1 snoop snoop  22327 May 25 14:11 Makefile
-rwxrwxr-x 1 snoop snoop 778881 May 25 14:14 moo
----------------------------------------------------------------------
```

The moo file is the one we care about, but the already-existing
restart.sh and README files are also interesting:

```bash
----------------------------------------------------------------------
-rw-rw-r-- 1 snoop snoop  10655 Jun 20  2008 README
-rw-rw-r-- 1 snoop snoop   2907 Jun 20  2008 README.Minimal
-rw-rw-r-- 1 snoop snoop  10121 Jun 20  2008 README.rX
-rwxrwxr-x 1 snoop snoop   2066 Jun 20  2008 restart
-rwxrwxr-x 1 snoop snoop   1808 Jun 20  2008 restart.sh
----------------------------------------------------------------------
```

Okay, before we run the MOO, let's fetch the moo database core we
want to run.

This MOO is starting with a RPG DB core, so we can just use wget to
fetch it directly, the same way we did with the lambda source.

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ cd
[snoop@portent ~]$ dir
lambdamoo-1.8.3  public_html  test  v1.8.3.tar
[snoop@portent ~]$ wget http://www.lisdude.com/moo/RPGCore.zip
--2015-05-25 14:22:07--  http://www.lisdude.com/moo/RPGCore.zip
Resolving www.lisdude.com... 192.95.30.126
Connecting to www.lisdude.com|192.95.30.126|:80... connected.
HTTP request sent, awaiting response... 200 OK
Length: 641612 (627K) [application/zip]
Saving to: `RPGCore.zip'

100%[===================================================================================================================================================================================================>] 641,612      737K/s   in 0.9s

2015-05-25 14:22:09 (737 KB/s) - `RPGCore.zip' saved [641612/641612]

[snoop@portent ~]$
----------------------------------------------------------------------
```

It's a zip file, so let's unzip it:

```bash
----------------------------------------------------------------------
[snoop@portent ~]$ unzip RPGCore.zip
Archive:  RPGCore.zip
   creating: RPG Core/
  inflating: RPG Core/ansihtml.txt
  inflating: RPG Core/citybldr.txt
  inflating: RPG Core/Files.txt
  inflating: RPG Core/patch__1.txt
  inflating: RPG Core/patch__2.txt
 extracting: RPG Core/rpgcore.zip
  inflating: RPG Core/schlbomb.txt
[snoop@portent ~]$
----------------------------------------------------------------------
```

I'm guessing the actual rpg core db is in rpgcore.zip, so:

```bash
----------------------------------Arcanus----------------------------------
[snoop@portent ~]$ cd RPG\ Core
[snoop@portent ~/RPG Core]$ dir
ansihtml.txt  citybldr.txt  Files.txt  patch__1.txt  patch__2.txt  rpgcore.zip  schlbomb.txt
[snoop@portent ~/RPG Core]$ unzip rpgcore.zip
Archive:  rpgcore.zip
  inflating: rpgcore.db
[snoop@portent ~/RPG Core]$
----------------------------------Arcanus----------------------------------
```

And there's the db.

So copy the rpgcore.db file into the lambdamoo-1.8.3 file:

```bash
----------------------------------------------------------------------
[snoop@portent ~/RPG Core]$ cp rpgcore.zip ~/lambdamoo-1.8.3
----------------------------------------------------------------------
```

Now remember that restart.sh file?  Change over to the lambdamoo
directory and run it, giving it "rpgcore" as an argument:

```bash
----------------------------------------------------------------------
[snoop@portent ~/RPG Core]$ cd ~/lambdamoo-1.8.3
[snoop@portent ~/lambdamoo-1.8.3]$ ./restart rpgcore
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

No output, but then restart.sh doesn't actually say anything if you
give it valid arguments.   However, there's now an rpgcore.log file:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ ls -al *.log
-rw-rw-r-- 1 snoop snoop 419 May 25 14:38 rpgcore.log
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

Let's look at the tail end of rpgcore.log:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ tail rpgcore.log
Mon May 25 14:38:54 MDT 2015: RESTARTED
May 25 14:38:54: NAME_LOOKUP: Started new lookup process
May 25 14:38:54: STARTING: Version 1.8.3 of the LambdaMOO server
May 25 14:38:54:           (Using BSD/TCP protocol)
May 25 14:38:54:           (Task timeouts measured in server CPU seconds.)
May 25 14:38:54: *** Binding listening socket: Address already in use
May 25 14:38:54: *** Can't create initial connection point!
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

We can see that moo had a problem binding to a port, so the default
moo port must already be in use by somebody.

We can use netstat to see what ports are currently in use:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ netstat -nlp
(No info could be read for "-p": geteuid()=152 but you should be root.)
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address               Foreign Address             State       PID/Program name   
tcp        0      0 0.0.0.0:4160                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:3200                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:6400                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:24800               0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:3201                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4321                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:7777                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2049                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:6402                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4226                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:9700                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2020                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4100                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4902                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:1190                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:11111               0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4423                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:7239                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2727                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4040                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:5800                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:3208                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2088                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4041                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2345                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4010                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:6250                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:6666                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:5803                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4140                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:9100                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4141                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:33805               0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:1038                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2095                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:655                 0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:111                 0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:7568                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:7760                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2800                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:6000                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2000                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4050                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:3666                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:626                 0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2099                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:5555                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2100                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2101                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2165                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2005                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4150                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:22                  0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2103                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:1399                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4919                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2007                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:23                  0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2200                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:3448                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2105                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:2169                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 127.0.0.1:25                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:1979                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4060                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 127.0.0.1:2812              0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:5309                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:3710                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:4222                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:830                 0.0.0.0:*                   LISTEN      -                   
tcp        0      0 0.0.0.0:5151                0.0.0.0:*                   LISTEN      -                   
tcp        0      0 :::8000                     :::*                        LISTEN      -                   
tcp        0      0 :::24800                    :::*                        LISTEN      -                   
tcp        0      0 :::6850                     :::*                        LISTEN      -                   
tcp        0      0 :::1220                     :::*                        LISTEN      -                   
tcp        0      0 :::1030                     :::*                        LISTEN      -                   
tcp        0      0 :::1190                     :::*                        LISTEN      -                   
tcp        0      0 :::2088                     :::*                        LISTEN      -                   
tcp        0      0 :::7561                     :::*                        LISTEN      -                   
tcp        0      0 :::1130                     :::*                        LISTEN      -                   
tcp        0      0 :::1035                     :::*                        LISTEN      -                   
tcp        0      0 :::1131                     :::*                        LISTEN      -                   
tcp        0      0 :::1132                     :::*                        LISTEN      -                   
tcp        0      0 :::1037                     :::*                        LISTEN      -                   
tcp        0      0 :::1421                     :::*                        LISTEN      -                   
tcp        0      0 :::80                       :::*                        LISTEN      -                   
tcp        0      0 :::2800                     :::*                        LISTEN      -                   
tcp        0      0 :::6000                     :::*                        LISTEN      -                   
tcp        0      0 :::4242                     :::*                        LISTEN      -                   
tcp        0      0 :::3666                     :::*                        LISTEN      -                   
tcp        0      0 :::9876                     :::*                        LISTEN      -                   
tcp        0      0 :::2165                     :::*                        LISTEN      -                   
tcp        0      0 :::21                       :::*                        LISTEN      -                   
tcp        0      0 :::22                       :::*                        LISTEN      -                   
tcp        0      0 :::1399                     :::*                        LISTEN      -                   
tcp        0      0 :::2200                     :::*                        LISTEN      -                   
tcp        0      0 :::2169                     :::*                        LISTEN      -                   
tcp        0      0 :::3449                     :::*                        LISTEN      -                   
tcp        0      0 :::1979                     :::*                        LISTEN      -                   
tcp        0      0 :::4222                     :::*                        LISTEN      -                   
udp        0      0 0.0.0.0:2049                0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:652                 0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:51617               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:59812               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:55980               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:40117               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:824                 0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:827                 0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:40000               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:59736               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:33883               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:53723               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:623                 0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:111                 0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:33265               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:39796               0.0.0.0:*                               -                   
udp        0      0 10.0.0.8:123                0.0.0.0:*                               -                   
udp        0      0 199.74.141.8:123            0.0.0.0:*                               -                   
udp        0      0 216.136.9.8:123             0.0.0.0:*                               -                   
udp        0      0 127.0.0.1:123               0.0.0.0:*                               -                   
udp        0      0 0.0.0.0:123                 0.0.0.0:*                               -                   
udp        0      0 fe80::20c:29ff:fe81:38d2:123 :::*                                    -                   
udp        0      0 2607:fc88:100:9::8:123      :::*                                    -                   
udp        0      0 fe80::20c:29ff:fe81:38dc:123 :::*                                    -                   
udp        0      0 ::1:123                     :::*                                    -                   
udp        0      0 :::123                      :::*                                    -                   
Active UNIX domain sockets (only servers)
Proto RefCnt Flags       Type       State         I-Node PID/Program name    Path
unix  2      [ ACC ]     STREAM     LISTENING     8066   -                   /var/run/dbus/system_bus_socket
unix  2      [ ACC ]     STREAM     LISTENING     8249   -                   /var/run/nscd/socket
unix  2      [ ACC ]     STREAM     LISTENING     9120   -                   /var/run/proftpd/proftpd.sock
unix  2      [ ACC ]     STREAM     LISTENING     61145844 -                   data/netmush.sock
unix  2      [ ACC ]     STREAM     LISTENING     20488  -                   data/netmush.sock
unix  2      [ ACC ]     STREAM     LISTENING     41017291 -                   data/netmush.sock
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

Let's try again, this time using port 1776, which netstat does not
show anyone else using.

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ ./restart.sh rpgcore 1776
[snoop@portent ~/lambdamoo-1.8.3]$
----------------------------------------------------------------------
```

So far, so good, but looking in the rpgcore.log again, the server had
problems with the RPG db core:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ tail -150f rpgcore.log
Mon May 25 14:50:21 MDT 2015: RESTARTED
May 25 14:50:21: CMDLINE: Initial port = 1776
May 25 14:50:21: NAME_LOOKUP: Started new lookup process
May 25 14:50:21: STARTING: Version 1.8.3 of the LambdaMOO server
May 25 14:50:21:           (Using BSD/TCP protocol)
May 25 14:50:21:           (Task timeouts measured in server CPU seconds.)
May 25 14:50:21: LOADING: rpgcore.db
May 25 14:50:21: *** DBIO_READ_NUM: Bad number: "2
" at file pos. 64
May 25 14:50:21: *** DBIO_READ_NUM: Bad number: "71
" at file pos. 68
May 25 14:50:21: *** DBIO_READ_NUM: Bad number: "36
" at file pos. 72
May 25 14:50:21: *** DBIO_READ_NUM: Bad number: "38
" at file pos. 76
May 25 14:50:21: *** DBIO_READ_NUM: Bad number: "159
" at file pos. 81
May 25 14:50:21: LOADING: Reading 164 objects...
May 25 14:50:21: *** READ_DB_FILE: Bad object #0.
May 25 14:50:21: *** DB_LOAD: Cannot load database!
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

Let's try it with the Minimal.db that comes with the source:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ ./restart.sh Minimal 1776
[snoop@portent ~/lambdamoo-1.8.3]$ ls -l *.log
-rw-rw-r-- 1 snoop snoop 1425 May 25 14:54 Minimal.log
-rw-rw-r-- 1 snoop snoop  878 May 25 14:50 rpgcore.log
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

Looks good, and:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ tail -150 Minimal.log 
Mon May 25 14:54:01 MDT 2015: RESTARTED
May 25 14:54:01: CMDLINE: Initial port = 1776
May 25 14:54:01: STARTING: Version 1.8.3 of the LambdaMOO server
May 25 14:54:01:           (Using BSD/TCP protocol)
May 25 14:54:01:           (Task timeouts measured in server CPU seconds.)
May 25 14:54:01: LOADING: Minimal.db
May 25 14:54:01: LOADING: Reading 4 objects...
May 25 14:54:01: LOADING: Done reading 4 objects ...
May 25 14:54:01: VALIDATING the object hierarchies ...
May 25 14:54:01: VALIDATE: Phase 1: Check for invalid objects ...
May 25 14:54:01: VALIDATE: Phase 2: Check for cycles ...
May 25 14:54:01: VALIDATE: Phase 3a: Finding delusional parents ...
May 25 14:54:01: VALIDATE: Phase 3b: Finding delusional children ...
May 25 14:54:01: VALIDATING the object hierarchies ... finished.
May 25 14:54:01: LOADING: Reading 1 MOO verb programs...
May 25 14:54:01: LOADING: Done reading 1 verb programs...
May 25 14:54:01: LOADING: Reading forked and suspended tasks...
May 25 14:54:01: LOADING: Reading list of formerly active connections...
May 25 14:54:01: LOADING: Minimal.db done, will dump new database on Minimal.db.new
May 25 14:54:01: INTERN: 0 allocations saved, 0 bytes
May 25 14:54:01: INTERN: at end, 6 entries in a 10007 bucket hash table.
May 25 14:54:01: Loaded protect cache for 128 builtins
May 25 14:54:01: LISTEN: #0 now listening on port 1776
May 25 14:54:01: NAME_LOOKUP: Started new lookup process
May 25 14:54:08: ACCEPT: #-2 on port 1776 from 50.153.237.134, port 32258
May 25 14:54:08: CONNECTED: Wizard (#3) on port 1776 from 50.153.237.134, port 32258
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

Yay, success!

Okay, what went wrong with rpgcore?

When in doubt, ask the Google.  I copied and pasted the
"DBIO_READ_NUM: Bad number" into a google search and found:

----------------------------------------------------------------------
http://www.chrisunkel.com/WinMOO/winmoo-faq.html

Q. When I take my database written by WinMOO, and try to load it into a LambdaMOO server under Unix, it doesn't work. What's going on?

A. Most likely, your database contains CRLF sequences (the Windows sequence) as the line-termination character, rather than the straight LF that Unix expects. Probably
this manifests as log messages that include something like:

```
"*** DBIO_READ_NUM: Bad number:". You need to remove the CRs.
```

There are many ways to do this. One is to FTP the file from the Windows machine to the Unix machine in ASCII rather than binary mode, which will perform appropriate translation of end-of-line sequences. Another is to strip out the CRs, e.g. with:

```bash
tr -d '\015' < foo-crlf.db > foo-lf.db
```

----------------------------------------------------------------------

"tr" is the unix Translate command.  Okay, let's give that a shot.

```bash
----------------------------------------------------------------------
[snoop@portent ~]$ cd RPG\ Core/
[snoop@portent ~/RPG Core]$ ls -al
total 2908
drwxrwxr-x  2 snoop snoop    1024 May 25 14:31 .
drwxrwx--- 10 snoop snoop    1024 May 25 15:05 ..
-rw-rw-r--  1 snoop snoop   18875 Dec 11  2007 ansihtml.txt
-rw-rw-r--  1 snoop snoop   18368 Dec 11  2007 citybldr.txt
-rw-rw-r--  1 snoop snoop    1168 Dec 11  2007 Files.txt
-rw-rw-r--  1 snoop snoop   26905 Dec 11  2007 patch__1.txt
-rw-rw-r--  1 snoop snoop    3606 Dec 11  2007 patch__2.txt
-rw-rw-r--  1 snoop snoop 2260369 Jul 16  2001 rpgcore.db
-rw-rw-r--  1 snoop snoop  618766 Dec 11  2007 rpgcore.zip
-rw-rw-r--  1 snoop snoop    4698 Dec 11  2007 schlbomb.txt
[snoop@portent ~/RPG Core]$ tr -d '\015' < rpgcore.db > rpgcore_nocrlf.db
[snoop@portent ~/RPG Core]$ cp rpgcore_nocrlf.db  ~/lambdamoo-1.8.3/
[snoop@portent ~/RPG Core]$ cd
[snoop@portent ~]$ cd lambdamoo-1.8.3/
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

Remember to shut down the current MOO server before restarting:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ ps -fu snoop
UID        PID  PPID  C STIME TTY          TIME CMD
snoop    18605 18602  0 13:54 ?        00:00:00 sshd: snoop@pts/13
snoop    18606 18605  0 13:54 pts/13   00:00:00 -tcsh
snoop    21324 21303  0 14:48 ?        00:00:00 sshd: snoop@pts/17
snoop    21325 21324  0 14:48 pts/17   00:00:00 -tcsh
snoop    21486     1  0 14:54 pts/17   00:00:00 ./moo Minimal.db Minimal.db.new
snoop    21489     1  0 14:54 pts/17   00:00:00 (MOO name-lookup master)
snoop    21491     1  0 14:54 pts/17   00:00:00 (MOO name-lookup slave)
snoop    21797 18606  0 15:04 pts/13   00:00:02 emacs -nw .
snoop    21863 21797  0 15:06 pts/27   00:00:00 -bin/tcsh -i
snoop    23388 21863  0 15:17 pts/27   00:00:00 ps -fu snoop
[snoop@portent ~/lambdamoo-1.8.3]$ kill 21486
[snoop@portent ~/lambdamoo-1.8.3]$ ps -fu snoop
UID        PID  PPID  C STIME TTY          TIME CMD
snoop    18605 18602  0 13:54 ?        00:00:00 sshd: snoop@pts/13
snoop    18606 18605  0 13:54 pts/13   00:00:00 -tcsh
snoop    21324 21303  0 14:48 ?        00:00:00 sshd: snoop@pts/17
snoop    21325 21324  0 14:48 pts/17   00:00:00 -tcsh
snoop    21797 18606  0 15:04 pts/13   00:00:02 emacs -nw .
snoop    21863 21797  0 15:06 pts/27   00:00:00 -bin/tcsh -i
snoop    23389 21863  0 15:17 pts/27   00:00:00 ps -fu snoop
[snoop@portent ~/lambdamoo-1.8.3]$ netstat -lnp | fgrep -i 1776
(No info could be read for "-p": geteuid()=152 but you should be root.)
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

Good, the way is clear, now:

```bash
----------------------------------------------------------------------
[snoop@portent ~/lambdamoo-1.8.3]$ ./restart.sh rpgcore_nocrlf 1776
[snoop@portent ~/lambdamoo-1.8.3]$ ls -l *.log
-rw-rw-r-- 1 snoop snoop 2189 May 25 15:17 Minimal.log
-rw-rw-r-- 1 snoop snoop  878 May 25 14:50 rpgcore.log
-rw-rw-r-- 1 snoop snoop 1469 May 25 15:17 rpgcore_nocrlf.log
[snoop@portent ~/lambdamoo-1.8.3]$ tail -150 rpgcore_nocrlf.log
Mon May 25 15:17:40 MDT 2015: RESTARTED
May 25 15:17:40: CMDLINE: Initial port = 1776
May 25 15:17:40: NAME_LOOKUP: Started new lookup process
May 25 15:17:40: STARTING: Version 1.8.3 of the LambdaMOO server
May 25 15:17:40:           (Using BSD/TCP protocol)
May 25 15:17:40:           (Task timeouts measured in server CPU seconds.)
May 25 15:17:40: LOADING: rpgcore_nocrlf.db
May 25 15:17:40: LOADING: Reading 164 objects...
May 25 15:17:40: LOADING: Done reading 164 objects ...
May 25 15:17:40: VALIDATING the object hierarchies ...
May 25 15:17:40: VALIDATE: Phase 1: Check for invalid objects ...
May 25 15:17:40: VALIDATE: Phase 2: Check for cycles ...
May 25 15:17:40: VALIDATE: Phase 3a: Finding delusional parents ...
May 25 15:17:40: VALIDATE: Phase 3b: Finding delusional children ...
May 25 15:17:40: VALIDATING the object hierarchies ... finished.
May 25 15:17:40: LOADING: Reading 2206 MOO verb programs...
May 25 15:17:40: LOADING: Done reading 2206 verb programs...
May 25 15:17:40: LOADING: Reading forked and suspended tasks...
May 25 15:17:40: LOADING: Reading list of formerly active connections...
May 25 15:17:40: LOADING: rpgcore_nocrlf.db done, will dump new database on rpgcore_nocrlf.db.new
May 25 15:17:40: INTERN: 20881 allocations saved, 217039 bytes
May 25 15:17:40: INTERN: at end, 18186 entries in a 20014 bucket hash table.
May 25 15:17:40: Loaded protect cache for 128 builtins
May 25 15:17:40: LISTEN: #0 now listening on port 1776
May 25 15:19:18: ACCEPT: #-2 on port 1776 from 50.153.237.134, port 32575
May 25 15:19:43: CONNECTED: Wizard (#2) on port 1776 from 50.153.237.134, port 32575
[snoop@portent ~/lambdamoo-1.8.3]$ 
----------------------------------------------------------------------
```

Success, yay!

## This file was modified from it's original source code to add markdown formatting. The original is available in the /src directory of this repo
