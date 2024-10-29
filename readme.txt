                                FAST WIPER

                Free space wiping tool by Popov Evgeniy Alekseyevich             

                           Version 1.1.1.2
System requirements

Operating system: Microsoft Windows NT 4.0 or higher
1 megabyte of free RAM

Program description

This program lets you wipe the free space on a storage media. It will be wiped by zero bytes.
A deleted file can't be recovered with special software after finishing wiping the free space.

License

This program is distributed under the GNU GENERAL PUBLIC LICENSE.

Usage program

This software is a console/text interface program.
It takes the amount of the wipe passes and a drive letter (for example, c) as the command-line arguments.
The amount of wipe pass is an optional argument. It is 1 by default.

Recommended amount of wipe passes

The recommended amount of the wipe passes depends on your paranoia level.
The wipe time also depends on the amount of wipe passes.

1: Normal for most cases.
3: Securely destroy important evidence.
5 or more: Total destroy all data. Also recommended for old hard drives.

File system compatibility

This program can't correctly work with FAT and FAT32.

Exit codes

0: Operation was successfully completed.
1: Can't create the temporary file.
2: Can't allocate memory.
3: Can't destroy the temporary file.
4: Can't decode a command-line argument.
5: Invalid amount of the wipe passes.
6: Can't create the temporary directory.
7: Can't destroy the temporary directory.

Source code

The code was compiled under Open Watcom V2 Fork.
But you can compile it under any modern C compiler.

Note for MINGW-w64 and TDM-GCC users

Don't forget to link your program with kernel32.lib.

Contact

You can send me a letter at tuzik87@inbox.ru.

Version history

0.1: Initial version.
0.2: The small changes.
0.3: The new command-line argument has been added.
0.4: The small changes. The first public version.
0.5: The memory usage has reduced.
0.6-0.7: The small changes.
0.7.0.1: The documentation has been updated.
0.7.1-0.7.4: The small changes.
0.7.5: The small bug has been fixed.
0.7.6-0.8: The small changes.
0.8.1: A small bug has been fixed.
0.8.2: The small changes.
0.8.2.1: The project file has been updated.
0.8.3-0.8.9: The small changes.
0.9: The source code was recompiled under Open Watcom V2 Fork.
0.9.1-0.9.2: The small changes.
0.9.2.1: The documentation has been updated.
0.9.3-0.9.5: The small changes.
0.9.6: The memory usage has reduced.
0.9.7: A command-line option has been changed.
0.9.8-0.9.9: The small changes.
1.0: The small bug has been fixed.
1.0.1-1.0.3: The small changes.
1.0.4: Visual C++ support has improved.
1.0.5-1.0.7: The small changes
1.0.8: A small bug has been fixed.
1.0.9: Tiny C Compiler support has been added.
1.1: The small changes.
1.1.0.1: The documentation has been updated.
1.1.1: The small changes.
1.1.1.1-1.1.1.2: The documentation has been updated.