#if defined _MSC_VER && _MSC_VER>=1400
  #pragma warning(disable : 4996)
#endif

#ifdef __GNUC__
 #define TROUBLE_COMPILER 1
#endif

#ifdef __TINYC__
 #define TROUBLE_COMPILER 1
#endif

#ifndef TROUBLE_COMPILER
 #pragma comment(lib,"kernel32.lib")
#endif

#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <direct.h>
#include <windows.h>