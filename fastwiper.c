#include "fastwiper.h"
#include "settings.h"

void show_intro();
void show_help();
void show_pass(const unsigned long int pass,const unsigned long int total);
void show_progress(const unsigned long long int start,const unsigned long long int end);
int create_temp_file(const char drive);
char *get_memory(const size_t length);
void remove_temp_file(const char drive);
unsigned long int decode_argument(const char *target);
void check_drive(const char *drive);
unsigned long int get_passes(const char *target);
void create_temp_directory(const char drive);
void remove_temp_directory(const char drive);
unsigned long long int get_wiping_size(const char drive);
void force_write(const int target,const size_t block,const size_t limit);
void corrupt_file(const int target,const unsigned long long int length);
void do_wipe(const unsigned long int passes,const char drive);
void work(const char *drive,const char *passes);

int main(int argc, char *argv[])
{
 show_intro();
 switch (argc)
 {
  case 3:
  work(argv[1],argv[2]);
  break;
  case 2:
  work(argv[1],"1");
  break;
  default:
  show_help();
  break;
 }
 return 0;
}

void show_intro()
{
 putchar('\n');
 puts("FAST WIPER");
 puts("Version 1.2.4");
 puts("The free space wiping tool by Popov Evgeniy Alekseyevich, 2016-2026 years");
 puts("This program is distributed under the GNU GENERAL PUBLIC LICENSE");
 putchar('\n');
}

void show_help()
{
 puts("You must give a drive letter and the number of the wipe passes as the command-line arguments!");
 puts("The number of the wipe passes is an optional argument. It is 1 by default");
}

void show_pass(const unsigned long int pass,const unsigned long int total)
{
 printf("The current wipe pass: %lu The total wipe passes: %lu",pass+1,total);
 putchar('\n');
}

void show_progress(const unsigned long long int start,const unsigned long long int end)
{
 putchar('\r');
 printf("The amount of the processed bytes: %llu from %llu. The progress:%llu%%",start,end,(start*100)/end);
}

int create_temp_file(const char drive)
{
 char name[]=TRASH_FILE;
 int target;
 name[0]=drive;
 target=open(name,O_CREAT|O_WRONLY|O_TRUNC|O_BINARY,S_IREAD|S_IWRITE);
 if(target==-1)
 {
  puts("Can't create the temporary file");
  exit(1);
 }
 return target;
}

char *get_memory(const size_t length)
{
 char *memory=NULL;
 memory=(char*)calloc(length,sizeof(char));
 if(memory==NULL)
 {
  puts("Can't allocate memory");
  exit(2);
 }
 return memory;
}

void remove_temp_file(const char drive)
{
 char name[]=TRASH_FILE;
 name[0]=drive;
 if (remove(name)!=0)
 {
  puts("Can't destroy the temporary file");
  exit(3);
 }

}

unsigned long int decode_argument(const char *target)
{
 size_t index,length;
 length=strlen(target);
 for (index=0;index<length;++index)
 {
  if (isdigit(target[index])==0)
  {
   puts("Can't decode a command-line argument");
   exit(4);
  }

 }
 return strtoul(target,NULL,10);
}

void check_drive(const char *drive)
{
 if (strlen(drive)!=1)
 {
  puts("Can't decode a command-line argument");
  exit(4);
 }
 if (isalpha(drive[0])==0)
 {
  puts("Can't decode a command-line argument");
  exit(4);
 }

}

unsigned long int get_passes(const char *target)
{
 unsigned long int passes;
 passes=decode_argument(target);
 if (passes==0)
 {
  puts("You must give a positive non-zero value as the number of the wipe passes");
  exit(5);
 }
 return passes;
}

void create_temp_directory(const char drive)
{
 char target[]=TRASH_DIRECTORY;
 target[0]=drive;
 if (mkdir(target)==-1)
 {
  puts("Can't create the temporary directory");
  exit(6);
 }

}

void remove_temp_directory(const char drive)
{
 char target[]=TRASH_DIRECTORY;
 target[0]=drive;
 if (rmdir(target)==-1)
 {
  puts("Can't destroy the temporary directory");
  exit(7);
 }

}

unsigned long long int get_wiping_size(const char drive)
{
 unsigned long long int length;
 char disk[]="a:\\";
 disk[0]=drive;
 if (GetDiskFreeSpaceExA((LPCSTR)disk,(PULARGE_INTEGER)&length,NULL,NULL)==FALSE)
 {
  length=0;
 }
 return length;
}

void force_write(const int target,const size_t block,const size_t limit)
{
 static size_t written=0;
 written+=block;
 if (written>=limit)
 {
  _commit(target);
  written=0;
 }

}

void corrupt_file(const int target,const unsigned long long int length)
{
 char *data=NULL;
 unsigned long long int index;
 size_t block;
 index=0;
 block=DATA_BLOCK_LENGTH;
 data=get_memory(block);
 while (index<length)
 {
  if ((length-index)<=DATA_BLOCK_LENGTH)
  {
   block=(size_t)(length-index);
  }
  if(write(target,data,block)==-1)
  {
   putchar('\n');
   puts("Can't totally wipe the free space");
   break;
  }
  index+=(unsigned long long int)block;
  show_progress(index,length);
  force_write(target,DATA_BLOCK_LENGTH,DATA_LIMIT);
 }
 free(data);
 putchar('\n');
 puts("Data synchronization in progress. Please wait");
 _commit(target);
 close(target);
}

void do_wipe(const unsigned long int passes,const char drive)
{
 unsigned long int index;
 puts("Wiping... Please wait");
 for (index=0;index<passes;++index)
 {
  show_pass(index,passes);
  corrupt_file(create_temp_file(drive),get_wiping_size(drive));
  remove_temp_file(drive);
 }
 puts("The wipe was successfully completed");
}

void work(const char *drive,const char *passes)
{
 check_drive(drive);
 create_temp_directory(drive[0]);
 do_wipe(get_passes(passes),drive[0]);
 remove_temp_directory(drive[0]);
}