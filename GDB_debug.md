# GDB调试

## 设置gdbinit初始环境

https://wizardforcel.gitbooks.io/100-gdb-tips/content/config-gdbinit.html

创建~/.gdbinit文件，添加如下内容

```shell
set environment LD_LIBRARY_PATH /usr/local/lib/

define bsave
    save breakpoints ~/.breakpoints
end

define brestore
   source ~/.breakpoints
end

# 保存历史命令
set history filename ~/.gdb_history
set history save on

# 退出时不显示提示信息
set confirm off

# 按照派生类型打印对象
set print object on

# 打印数组的索引下标
set print array-indexes on

# 每行打印一个结构体成员
set print pretty on
set history save on
set history size unlimited
set history remove-duplicates unlimited
set history expansion on
```



## 设置断点

```shell
(gdb) break /Full/path/to/service.cpp:45
```

## 保存断点

https://stackoverflow.com/questions/501486/getting-gdb-to-save-a-list-of-breakpoints

As of GDB 7.2 (2011-08-23) you can now use the *[save breakpoints](https://sourceware.org/gdb/onlinedocs/gdb/Save-Breakpoints.html)* command.

```vhdl
(gdb) save breakpoints <filename>
  Save all current breakpoint definitions to a file suitable for use
  in a later debugging session.  To read the saved breakpoint
  definitions, use the `source' command.
```

Use source command  to restore the saved breakpoints from the file.

```shell
(gdb) source  <filename>
```

测试

```shell(gdb) source br_file
(gdb) save breakpoints br_file
Saved to file 'br_file'.
(gdb) source br_file
Breakpoint 1 at 0x43ccea: file /home/zq1219/targetNFS/zq_scp_ecg/TestScpEcgWriter.cpp, line 219.
Breakpoint 2 at 0x43ccf6: file /home/zq1219/targetNFS/zq_scp_ecg/TestScpEcgWriter.cpp, line 220.
```

## 删除断点

```shell
clear
clear function
clear filename:function
clear linenum
clear filename:linenum
delete [breakpoints] [range...]
d    #清空所有
```

## 查看断点

设置记录日志

```delphi
(gdb) b main
Breakpoint 1 at 0x8049329
(gdb) info break
Num     Type           Disp Enb Address    What
1       breakpoint     keep y   0x08049329 <main+16>
(gdb) set logging file breaks.txt
(gdb) set logging on
Copying output to breaks.txt.
(gdb) info break
Num     Type           Disp Enb Address    What
1       breakpoint     keep y   0x08049329 <main+16>
(gdb) q
```

```shell
(gdb) i b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000000000043ccea in main() at /home/zq1219/targetNFS/zq_scp_ecg/TestScpEcgWriter.cpp:219
2       breakpoint     keep y   0x000000000043ccf6 in main() at /home/zq1219/targetNFS/zq_scp_ecg/TestScpEcgWriter.cpp:220
```

## 查看变量值

1、每次停在断点时，自动打印变量值

Use the `display` command:

```
(gdb> display decoder.m_msg
```

2、打印所有变量

This will cause `decoder.m_msg` to be printed every time that the prompt is shown (not only after a breakpoint).

Type [`info variables`](http://sourceware.org/gdb/current/onlinedocs/gdb/Symbols.html#index-info-variables-918) to list "All global and static variable names".

Type [`info locals`](http://sourceware.org/gdb/current/onlinedocs/gdb/Frame-Info.html#index-info-locals-435) to list "Local variables of current stack frame" (names and values), including static variables in that function.

Type [`info args`](https://sourceware.org/gdb/current/onlinedocs/gdb/Frame-Info.html#index-info-args) to list "Arguments of the current stack frame" (names and values).

## 常用命令

| cmd                                                          | comment                                                      |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| r[un]                                                        | Run to next [breakpoint](https://ccrma.stanford.edu/~jos/sasp/Envelope_Compression.html) or to end |
| s[tep]                                                       | Single-step, descending into functions                       |
| n[ext]                                                       | Single-step *without* descending into functions              |
| fin[ish]                                                     | Finish current function, loop, etc. (useful!)                |
| c[ontinue]                                                   | Continue to next breakpoint or end                           |
| up                                                           | Go up one context level on stack (to caller)                 |
| do[wn]                                                       | Go down one level (only possible after `up`)                 |
| l[ist]                                                       | Show lines of code surrounding the current point             |
| p[rint] ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) | Print value of variable called ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) |
| p ![$ \ast<$](https://ccrma.stanford.edu/~jos/stkintro/img3.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) | Print what is pointed to by ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) |
| p/x ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) | Print value of ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) in hex format |
| p ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) @![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) n![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) | print ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) n![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) values starting at ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) |
| p ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) chars![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) tab![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) | List all variables starting with ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) chars![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) |
| x/s   &Sample                                                | prints the whole string                                      |
| x/3c  &Sample                                                | prints: "T" "h" "i"                                          |
| b[reak] ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) | Set a breakpoint at function ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) |
| b ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) class![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) ::![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) | Set a breakpoint at ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) name![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) in ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) class![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) |
| b ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) class![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) ::![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) tab![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) | List all members in ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) class![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) |
| h[elp] b                                                     | Documentation for setting breakpoints                        |
| i[nfo] b                                                     | List breakpoints                                             |
| i                                                            | List all info commands                                       |
| dis[able] 1                                                  | Disable breakpoint 1                                         |
| en[able] 1                                                   | Enable breakpoint 1                                          |
| d[elete] 1                                                   | Delete breakpoint 1                                          |
| d 1 2                                                        | Delete breakpoints 1 and 2                                   |
| d                                                            | Delete all breakpoints                                       |
| cond[ition] 1 ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) expr![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) | Stop at breakpoint 1 only if ![$ <$](https://ccrma.stanford.edu/~jos/stkintro/img1.png) expr![$ >$](https://ccrma.stanford.edu/~jos/stkintro/img2.png) is true |
| cond 1                                                       | Make breakpoint 1 unconditional                              |
| comm[ands] 1                                                 | Add a list of `gdb` commands to execute                      |
|                                                              | each time breakpoint 1 is hit                                |
|                                                              | (usually just `print <var>`)                                 |

# 遇到错误

## 1、C库函数符号找不到

https://stackoverflow.com/questions/48278881/gdb-complaining-about-missing-raise-c/48287761#48287761

```cpp
Program received signal SIGABRT, Aborted.
0x76cd0f70 in __GI_raise (sig=sig@entry=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
56  ../nptl/sysdeps/unix/sysv/linux/raise.c: No such file or directory.
```

```shell
#查看当前运行的源文件
(gdb) info source
Current source file is ../sysdeps/unix/sysv/linux/raise.c
Compilation directory is /build/glibc-S7Ft5T/glibc-2.23/signal
Source language is c.
Producer is GNU C11 5.4.0 20160609 -mtune=generic -march=x86-64 -g -O2 -O3 -std=gnu11 -fgnu89-inline -fno-stack-protector -fmerge-all-constants -frounding-math -fPIC -ftls-model=initial-exec.
Compiled with DWARF 2 debugging format.
Does not include preprocessor macro info.
```

解决办法

```shell
sudo mkdir /opt/src
cd /opt/src/
sudo apt source libc6
find $PWD -maxdepth 1 -type d -name "glibc*"
ls /opt/src/glibc-2.23/sysdeps/unix/sysv/linux/raise.c 
```

设置库路径

```cpp
(gdb) set substitute-path /build/glibc-S7Ft5T/glibc-2.23 /opt/src/glibc-2.23
```

## 2、查找不到库文件

https://stackoverflow.com/questions/16254546/gdb-can-not-open-shared-object-file

```shell
(gdb) r
Starting program: /home/zq1219/targetNFS/zq_scp_ecg/build/scp_ecg_writer 
/home/zq1219/targetNFS/zq_scp_ecg/build/scp_ecg_writer: error while loading shared libraries: libiconv.so.2: cannot open shared object file: No such file or directory
[Inferior 1 (process 39876) exited with code 0177]
```

```shell
(gdb) set env LD_LIBRARY_PATH /usr/local/lib/
(gdb) set environment LD_LIBRARY_PATH /usr/local/lib/
```

## 3、运行时pure virtual method called

https://mine260309.me/archives/1051

https://stackoverflow.com/questions/5407304/error-pure-virtual-method-called-terminate-called-without-an-active-exception

```shell
(gdb) n
ECGConversion::SCP::SCPSection::SCPSection (this=0x688c20) at /home/zq1219/targetNFS/zq_scp_ecg/SCP/SCPSection.cpp:21
21          SectionID = getSectionID();
(gdb) n
22          Empty();
(gdb) n
pure virtual method called
terminate called without an active exception
```

解决办法：

在构造函数和析构函数中不能调用纯虚函数

Usually you get this error when call your virtual function from constructor or destructor. Check that that is not the case.