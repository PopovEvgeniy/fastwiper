#if defined _MSC_VER && _MSC_VER>=1400
  #pragma warning(disable : 4996)
#endif

#ifndef __GNUC__
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