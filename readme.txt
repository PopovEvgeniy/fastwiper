                                FAST WIPER

                Free space wiping tool by Popov Evgeniy Alekseyevich             

                           Version 1.1
System requirement

Operating system: Microsoft Windows NT 4.0 or higher
1 megabyte of free RAM

Program description

This program let you wipe free space on storage media. It will wipe by zero bytes.
Deleted files can't be recovered with special software after finish wipe free space.

License

This program distributed under GNU GENERAL PUBLIC LICENSE.

Usage program

This software is a console/text interface program.
It takes the amount of wipe pass and drive letter (for example:c) as command line argument.
Amount of wipe pass is optional argument. Amount of wipe pass is 1 by default.

Recommend the amount of wipe pass

Recommend the amount of wipe pass to depend on your paranoia level. ;)
Wipe time also depend on the amount of wipe pass.

1 - Normal for most case.
3 - Securely destroy important evidence.
5 or more - Total destroy all shit. Also recommend for old hard drives.

File system compatibility

This program can't correctly work with FAT and FAT32.

Exit codes

0 - Operation successfully complete.
1 - Can't create the temporary file.
2 - Can't allocate memory.
3 - Can't destroy the temporary file.
4 - Can't decode command line argument.
5 - Invalid amount of wipe pass.
6 - Can't create temporary directory.
7 - Can't destroy temporary directory.

Source code

The source code compiled under Open Watcom V2 Fork.
But you can compile it under any modern C compiler.

Note for MINGW�w64 and TDM�GCC users

Don't forget to link your program with kernel32.lib.

Contact

You can send me a letter to tuzik87@inbox.ru

Version history

0.1 - Initial version
0.2 - Small changes
0.3 - New command line argument added
0.4 - Small changes. First public version
0.5 - Memory usage reduced
0.6 - 0.7 - Small changes
0.7.0.1 � Documentation updated
0.7.1 - 0.7.4 - Small changes
0.7.5 - Small bug fixed
0.7.6 - 0.8 - Small changes
0.8.1 - Small bug fixed
0.8.2 - Small changes
0.8.2.1 - Project file updated
0.8.3 - 0.8.9 - Small changes
0.9 - Source code recompiled under Open Watcom V2 Fork
0.9.1 - 0.9.2 - Small changes
0.9.2.1 � Documentation updated
0.9.3 - 0.9.5 - Small changes
0.9.6 - Memory usage reduced
0.9.7 - Command line options changed
0.9.8 - 0.9.9 - Small changes
1.0 - Small bug fixed
1.0.1 - 1.0.3 - Small changes
1.0.4 - Visual C++ support improved
1.0.5 - 1.0.7 - Small changes
1.0.8 - Small bug fixed
1.0.9 - Tiny C Compiler support added
1.1 - Small changes