[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/ZhE-zmlt)
# Project 1
Answer these questions please:

Project Overview:
-----------------------------------
What is the project about? Provide a brief overview that includes the project's name, purpose, and target audience.

Project 1 is meant to teach us about how a shell works, by having us implement a rudimentary one of our one, while simultaneosuly serving as a warm up for future projects that will use low level C programming. The project is aimed at someone who is already familiar with C, so that C can be used as a mean of learning about the shell/operating system rather than serving as a barrier of entry.

Solution Approach:
------------------------------------------------------------
How did you approach the problem? Outline the steps or phases in your project development process?

The project was laid out into very clear steps, which created a clear framework for how to develop the project. I would code a part, push and move it over to the VM to test and debug, and then move on to the next part once the previous one was functioning. The first step given was a bit lengthy, so I had to split it into sections for implementing and testing user_prompt_loop, get_user_command, and parse_command

Challenges and Solutions?
---------------------------
What challenges did you encounter while completeing your project, and how did you overcome them?

Most of my issues came with the parse_command and history implementations. The issues with the parse_command were mostly due to my misunderstanding the util functions given and then putting myself into infinite loops by accident. The history issues were far more strange, having to do with fopen getting very confused when repeatedly calling getenv, as well as having issues with cursor placement when using fseek and fputs.
My biggest friends were gdb and valgrind. Enabling the extra checks on valgrind did a lot of the work for me, since it'd tell me exactly which variable was having an issue, exactly where it was used, and exactly where it was created. Other than that, stepping through with gdb was quite useful for seeing exactly what was in each of the local variables every step of the way when a function was called, essentially allowing me to add print statements wherever I want on the fly, particularly useful when trying to figure out the return values from the util functions.

LLM Disclosure
--------------
Are there any external resources or references that might be helpful?

I used tutorialspoint.com in order to learn about any of the declarations, return values, or other functionalities of any C library functions I needed a refresher on.

Testing and Validation
-----------------------
Testing Methodology: How did you test your project? Discuss any unit tests, integration tests, or user acceptance testing conducted.
Are there any specific commands or scripts that should be known?

I used the project1-all.mov posted by Sanaa as a baseline for my testing. I would run each of the commands in the video, including all the proc commands, ls, mkdir, cp, history, and exit, using gdb and valgrind and made sure I got the same/similar output.
It should be noted that the schema for calling proc is just as in the video, ie $/proj /cpuinfo OR $/proj /pid/status

Example session
---------------
Provide examples on running your project

jay@debian:~/project1-Quaver21$ make clean
rm -f simple_shell.o utils.o
rm -f simple_shell
jay@debian:~/project1-Quaver21$ make
cc -c simple_shell.c -o simple_shell.o -g -Wall -Wextra -pedantic -std=gnu11
simple_shell.c: In function ‘main’:
simple_shell.c:36:27: warning: unused parameter ‘argv’ [-Wunused-parameter]
   36 | int main(int argc, char **argv) {
      |                    ~~~~~~~^~~~
cc -c utils.c -o utils.o -g -Wall -Wextra -pedantic -std=gnu11
utils.c: In function ‘unescape’:
utils.c:62:31: warning: unused variable ‘i’ [-Wunused-variable]
   62 |     size_t len = strlen(str), i;
      |                               ^
cc -o simple_shell simple_shell.o utils.o -g
jay@debian:~/project1-Quaver21$ ./simple_shell
$ ls 
Makefile  README.md  simple_shell  simple_shell.c  simple_shell.o  utils.c  utils.h  utils.o
$ ls -l
total 108
-rw-r--r-- 1 jay jay   781 Mar  9 14:21 Makefile
-rw-r--r-- 1 jay jay  3518 Mar 10 14:57 README.md
-rwxr-xr-x 1 jay jay 31360 Mar 10 15:02 simple_shell
-rw-r--r-- 1 jay jay 13124 Mar 10 14:37 simple_shell.c
-rw-r--r-- 1 jay jay 21736 Mar 10 15:02 simple_shell.o
-rw-r--r-- 1 jay jay  7714 Mar  9 14:21 utils.c
-rw-r--r-- 1 jay jay  2330 Mar  9 14:21 utils.h
-rw-r--r-- 1 jay jay 13984 Mar 10 15:02 utils.o
$ ls -a
.  ..  .git  Makefile  README.md  simple_shell	simple_shell.c	simple_shell.o	utils.c  utils.h  utils.o
$ pwd
/home/jay/project1-Quaver21
$ mkdir testFolder
$ ls
Makefile  README.md  simple_shell  simple_shell.c  simple_shell.o  testFolder  utils.c	utils.h  utils.o
$ cp Makfile tetFolder
cp: cannot stat 'Makfile': No such file or directory
$ cp Makfile testFolder
cp: cannot stat 'Makfile': No such file or directory
$ cp Makefile testFolder
$ cat testFolder/Makefile
# Add the names of your .c files from your assignment here.
SRCS = simple_shell.c utils.c

# This line is the name of the program you are building
BIN = simple_shell

# Compiler flags to pass to the system's C compiler while building
# the source files
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu11

# Flags to pass to the linker while linking your program
# You shouldn't need to change this at all.
LDFLAGS = -g

###################################################################
# You shouldn't need to touch anything below this line...         #
###################################################################
OBJS = $(SRCS:.c=.o)

all: $(BIN)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	-rm -f $(OBJS)
	-rm -f $(BIN)
$ rmdir testFolder
rmdir: failed to remove 'testFolder': Directory not empty
$ rmdir -r testFolder
rmdir: invalid option -- 'r'
Try 'rmdir --help' for more information.
$ ps
    PID TTY          TIME CMD
   1910 pts/0    00:00:00 bash
   3290 pts/0    00:00:00 simple_shell
   3303 pts/0    00:00:00 ps
$ /proc /cpuinfo
processor	: 0
vendor_id	: GenuineIntel
cpu family	: 6
model		: 158
model name	: Intel(R) Core(TM) i7-9700F CPU @ 3.00GHz
stepping	: 13
cpu MHz		: 3000.000
cache size	: 12288 KB
physical id	: 0
siblings	: 6
core id		: 0
cpu cores	: 6
apicid		: 0
initial apicid	: 0
fpu		: yes
fpu_exception	: yes
cpuid level	: 22
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3 cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single fsgsbase avx2 invpcid rdseed clflushopt md_clear flush_l1d arch_capabilities
bugs		: spectre_v1 spectre_v2 spec_store_bypass swapgs taa itlb_multihit srbds mmio_stale_data retbleed
bogomips	: 6000.00
clflush size	: 64
cache_alignment	: 64
address sizes	: 39 bits physical, 48 bits virtual
power management:

processor	: 1
vendor_id	: GenuineIntel
cpu family	: 6
model		: 158
model name	: Intel(R) Core(TM) i7-9700F CPU @ 3.00GHz
stepping	: 13
cpu MHz		: 3000.000
cache size	: 12288 KB
physical id	: 0
siblings	: 6
core id		: 1
cpu cores	: 6
apicid		: 1
initial apicid	: 1
fpu		: yes
fpu_exception	: yes
cpuid level	: 22
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3 cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single fsgsbase avx2 invpcid rdseed clflushopt md_clear flush_l1d arch_capabilities
bugs		: spectre_v1 spectre_v2 spec_store_bypass swapgs taa itlb_multihit srbds mmio_stale_data retbleed
bogomips	: 6000.00
clflush size	: 64
cache_alignment	: 64
address sizes	: 39 bits physical, 48 bits virtual
power management:

processor	: 2
vendor_id	: GenuineIntel
cpu family	: 6
model		: 158
model name	: Intel(R) Core(TM) i7-9700F CPU @ 3.00GHz
stepping	: 13
cpu MHz		: 3000.000
cache size	: 12288 KB
physical id	: 0
siblings	: 6
core id		: 2
cpu cores	: 6
apicid		: 2
initial apicid	: 2
fpu		: yes
fpu_exception	: yes
cpuid level	: 22
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3 cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single fsgsbase avx2 invpcid rdseed clflushopt md_clear flush_l1d arch_capabilities
bugs		: spectre_v1 spectre_v2 spec_store_bypass swapgs taa itlb_multihit srbds mmio_stale_data retbleed
bogomips	: 6000.00
clflush size	: 64
cache_alignment	: 64
address sizes	: 39 bits physical, 48 bits virtual
power management:

processor	: 3
vendor_id	: GenuineIntel
cpu family	: 6
model		: 158
model name	: Intel(R) Core(TM) i7-9700F CPU @ 3.00GHz
stepping	: 13
cpu MHz		: 3000.000
cache size	: 12288 KB
physical id	: 0
siblings	: 6
core id		: 3
cpu cores	: 6
apicid		: 3
initial apicid	: 3
fpu		: yes
fpu_exception	: yes
cpuid level	: 22
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3 cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single fsgsbase avx2 invpcid rdseed clflushopt md_clear flush_l1d arch_capabilities
bugs		: spectre_v1 spectre_v2 spec_store_bypass swapgs taa itlb_multihit srbds mmio_stale_data retbleed
bogomips	: 6000.00
clflush size	: 64
cache_alignment	: 64
address sizes	: 39 bits physical, 48 bits virtual
power management:

processor	: 4
vendor_id	: GenuineIntel
cpu family	: 6
model		: 158
model name	: Intel(R) Core(TM) i7-9700F CPU @ 3.00GHz
stepping	: 13
cpu MHz		: 3000.000
cache size	: 12288 KB
physical id	: 0
siblings	: 6
core id		: 4
cpu cores	: 6
apicid		: 4
initial apicid	: 4
fpu		: yes
fpu_exception	: yes
cpuid level	: 22
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3 cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single fsgsbase avx2 invpcid rdseed clflushopt md_clear flush_l1d arch_capabilities
bugs		: spectre_v1 spectre_v2 spec_store_bypass swapgs taa itlb_multihit srbds mmio_stale_data retbleed
bogomips	: 6000.00
clflush size	: 64
cache_alignment	: 64
address sizes	: 39 bits physical, 48 bits virtual
power management:

processor	: 5
vendor_id	: GenuineIntel
cpu family	: 6
model		: 158
model name	: Intel(R) Core(TM) i7-9700F CPU @ 3.00GHz
stepping	: 13
cpu MHz		: 3000.000
cache size	: 12288 KB
physical id	: 0
siblings	: 6
core id		: 5
cpu cores	: 6
apicid		: 5
initial apicid	: 5
fpu		: yes
fpu_exception	: yes
cpuid level	: 22
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3 cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single fsgsbase avx2 invpcid rdseed clflushopt md_clear flush_l1d arch_capabilities
bugs		: spectre_v1 spectre_v2 spec_store_bypass swapgs taa itlb_multihit srbds mmio_stale_data retbleed
bogomips	: 6000.00
clflush size	: 64
cache_alignment	: 64
address sizes	: 39 bits physical, 48 bits virtual
power management:

$ /proc /3290/status
Name:	simple_shell
Umask:	0022
State:	R (running)
Tgid:	3290
Ngid:	0
Pid:	3290
PPid:	1910
TracerPid:	0
Uid:	1000	1000	1000	1000
Gid:	1000	1000	1000	1000
FDSize:	256
Groups:	24 25 27 29 30 44 46 108 113 117 120 1000 
NStgid:	3290
NSpid:	3290
NSpgid:	3290
NSsid:	1910
VmPeak:	    2368 kB
VmSize:	    2368 kB
VmLck:	       0 kB
VmPin:	       0 kB
VmHWM:	    1364 kB
VmRSS:	    1364 kB
RssAnon:	      80 kB
RssFile:	    1284 kB
RssShmem:	       0 kB
VmData:	     176 kB
VmStk:	     132 kB
VmExe:	       8 kB
VmLib:	    1516 kB
VmPTE:	      44 kB
VmSwap:	       0 kB
HugetlbPages:	       0 kB
CoreDumping:	0
THP_enabled:	1
Threads:	1
SigQ:	0/31600
SigPnd:	0000000000000000
ShdPnd:	0000000000000000
SigBlk:	0000000000000000
SigIgn:	0000000000000000
SigCgt:	0000000000000000
CapInh:	0000000000000000
CapPrm:	0000000000000000
CapEff:	0000000000000000
CapBnd:	000001ffffffffff
CapAmb:	0000000000000000
NoNewPrivs:	0
Seccomp:	0
Seccomp_filters:	0
Speculation_Store_Bypass:	vulnerable
SpeculationIndirectBranch:	always enabled
Cpus_allowed:	3f
Cpus_allowed_list:	0-5
Mems_allowed:	00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000001
Mems_allowed_list:	0
voluntary_ctxt_switches:	15
nonvoluntary_ctxt_switches:	3
$ /proc /3290/environ
SHELL=/bin/bashSESSION_MANAGER=local/debian:@/tmp/.ICE-unix/1275,unix/debian:/tmp/.ICE-unix/1275QT_ACCESSIBILITY=1COLORTERM=truecolorSSH_AGENT_LAUNCHER=opensshXDG_MENU_PREFIX=gnome-GNOME_DESKTOP_SESSION_ID=this-is-deprecatedSSH_AUTH_SOCK=/run/user/1000/keyring/sshXMODIFIERS=@im=ibusDESKTOP_SESSION=gnomeGTK_MODULES=gail:atk-bridgePWD=/home/jay/project1-Quaver21LOGNAME=jayXDG_SESSION_DESKTOP=gnomeXDG_SESSION_TYPE=waylandXAUTHORITY=/run/user/1000/.mutter-Xwaylandauth.3N95J2GDM_LANG=en_US.UTF-8HOME=/home/jay/.421shE=jayIM_CONFIG_PHASE=1LANG=en_US.UTF-8LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:XDG_CURRENT_DESKTOP=GNOMEVTE_VERSION=6203WAYLAND_DISPLAY=wayland-0GNOME_TERMINAL_SCREEN=/org/gnome/Terminal/screen/25469ac9_ef1a_4ea2_a768_d6d1a488458aGNOME_SETUP_DISPLAY=:1XDG_SESSION_CLASS=userTERM=xterm-256colorUSER=jayGNOME_TERMINAL_SERVICE=:1.98DISPLAY=:0SHLVL=1QT_IM_MODULE=ibusXDG_RUNTIME_DIR=/run/user/1000PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/gamesGDMSESSION=gnomeDBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/busOLDPWD=/home/jay_=./simple_shell$             
$ /proc /3290/sched
simple_shell (3290, #threads: 1)
-------------------------------------------------------------------
se.exec_start                                :       5952621.066217
se.vruntime                                  :          2581.005613
se.sum_exec_runtime                          :            22.459927
se.nr_migrations                             :                    2
nr_switches                                  :                   21
nr_voluntary_switches                        :                   18
nr_involuntary_switches                      :                    3
se.load.weight                               :              1048576
se.avg.load_sum                              :                    0
se.avg.runnable_sum                          :                    0
se.avg.util_sum                              :                    0
se.avg.load_avg                              :                    0
se.avg.runnable_avg                          :                    0
se.avg.util_avg                              :                    0
se.avg.last_update_time                      :        5952621065216
se.avg.util_est.ewma                         :                   41
se.avg.util_est.enqueued                     :                    0
policy                                       :                    0
prio                                         :                  120
clock-delta                                  :                   42
mm->numa_scan_seq                            :                    0
numa_pages_migrated                          :                    0
numa_preferred_nid                           :                   -1
total_numa_faults                            :                    0
current_node=0, numa_group_id=0
numa_faults node=0 task_private=0 task_shared=0 group_private=0 group_shared=0
$ /proc /loadavg 
0.02 0.08 0.13 2/678 3304
$ history
cp Makefile testFolder
cat testFolder/Makefile
rmdir testFolder
rmdir -r testFolder
ps
/proc /cpuinfo
/proc /3290/status
/proc /3290/environ
/proc /3290/sched
/proc /loadavg
$ exit
jay@debian:~/project1-Quaver21$ echo $?
0
jay@debian:~/project1-Quaver21$ ./simple_shell
$ exit 56
jay@debian:~/project1-Quaver21$ echo $?
56
jay@debian:~/project1-Quaver21$ valgrind ./simple_shell
==3309== Memcheck, a memory error detector
==3309== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==3309== Using Valgrind-3.16.1 and LibVEX; rerun with -h for copyright info
==3309== Command: ./simple_shell
==3309== 
$ Valgrind -h
execvp error: No such file or directory
==3311== 
==3311== HEAP SUMMARY:
==3311==     in use at exit: 0 bytes in 0 blocks
==3311==   total heap usage: 10 allocs, 10 frees, 2,608 bytes allocated
==3311== 
==3311== All heap blocks were freed -- no leaks are possible
==3311== 
==3311== For lists of detected and suppressed errors, rerun with: -s
==3311== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
$ echo "Hello world"
Hello world
$ echo "Hello world
shell error: unterminated quote
$ exit
==3309== 
==3309== HEAP SUMMARY:
==3309==     in use at exit: 0 bytes in 0 blocks
==3309==   total heap usage: 34 allocs, 34 frees, 16,592 bytes allocated
==3309== 
==3309== All heap blocks were freed -- no leaks are possible
==3309== 
==3309== For lists of detected and suppressed errors, rerun with: -s
==3309== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)