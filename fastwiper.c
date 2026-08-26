#include "fastwiper.h"
#include "exitcode.h"
#include "settings.h"

void show_intro();
void show_help();
void show_pass(const unsigned long int pass,const unsigned long int total);
void show_progress(const unsigned long long int start,const unsigned long long int end);
void show_error(const char *message);
int create_temp_file(const char drive);
unsigned char *get_memory(const size_t length);
void remove_temp_file(const char drive);
unsigned long int decode_argument(const char *target);
void check_drive(const char *drive);
unsigned long int get_passes(const char *target);
void create_temp_directory(const char drive);
void remove_temp_directory(const char drive);
unsigned long long int get_free_space(const char drive);
size_t write_data(const int target,const unsigned char *buffer,const size_t length);
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
  exit(COMMAND_LINE_ARGUMENTS_ERROR);
  break;
 }
 return 0;
}

void show_intro()
{
 putchar('\n');
 puts("FAST WIPER");
 puts("Version 1.3.6");
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

void show_error(const char *message)
{
 fputc('\n',stderr);
 fputs(message,stderr);
 fputc('\n',stderr);
}

int create_temp_file(const char drive)
{
 int target=-1;
 char name[]=TRASH_FILE;
 name[0]=drive;
 target=open(name,O_CREAT|O_WRONLY|O_TRUNC|O_BINARY,S_IREAD|S_IWRITE);
 if (target==-1)
 {
  show_error("Can't create the temporary file");
  exit(CREATE_FILE_ERROR);
 }
 return target;
}

unsigned char *get_memory(const size_t length)
{
 unsigned char *memory=NULL;
 memory=(unsigned char*)calloc(length,sizeof(char));
 if (memory==NULL)
 {
  show_error("Can't allocate memory");
  exit(MEMORY_ALLOCATION_ERROR);
 }
 return memory;
}

void remove_temp_file(const char drive)
{
 char name[]=TRASH_FILE;
 name[0]=drive;
 if (remove(name)!=0)
 {
  show_error("Can't destroy the temporary file");
  exit(DESTROY_FILE_ERROR);
 }

}

unsigned long int decode_argument(const char *target)
{
 unsigned long int argument=0;
 size_t index=0;
 size_t length=0;
 if (target!=NULL)
 {
  length=strlen(target);
 }
 if (length==0)
 {
  show_error("Can't decode a command-line argument");
  exit(DECODE_ARGUMENT_ERROR);
 }
 for (index=0;index<length;++index)
 {
  if (isdigit(target[index])==0)
  {
   show_error("Can't decode a command-line argument");
   exit(DECODE_ARGUMENT_ERROR);
  }

 }
 argument=strtoul(target,NULL,10);
 if (errno==ERANGE)
 {
  show_error("Can't decode a command-line argument");
  exit(DECODE_ARGUMENT_ERROR);
 }
 return argument;
}

void check_drive(const char *drive)
{
 size_t length=0;
 if (drive!=NULL)
 {
  length=strlen(drive);
 }
 if (length!=1)
 {
  show_error("Can't decode a command-line argument");
  exit(DECODE_ARGUMENT_ERROR);
 }
 if (isalpha(drive[0])==0)
 {
  show_error("Can't decode a command-line argument");
  exit(DECODE_ARGUMENT_ERROR);
 }

}

unsigned long int get_passes(const char *target)
{
 unsigned long int passes=0;
 passes=decode_argument(target);
 if (passes==0)
 {
  show_error("You must give a positive non-zero value as the number of the wipe passes");
  exit(INVALID_PASSES_ERROR);
 }
 return passes;
}

void create_temp_directory(const char drive)
{
 char target[]=TRASH_DIRECTORY;
 target[0]=drive;
 if (mkdir(target)==-1)
 {
  show_error("Can't create the temporary directory");
  exit(CREATE_DIRECTORY_ERROR);
 }

}

void remove_temp_directory(const char drive)
{
 char target[]=TRASH_DIRECTORY;
 target[0]=drive;
 if (rmdir(target)==-1)
 {
  show_error("Can't destroy the temporary directory");
  exit(DESTROY_DIRECTORY_ERROR);
 }

}

unsigned long long int get_free_space(const char drive)
{
 char disk[]="a:\\";
 ULARGE_INTEGER space;
 disk[0]=drive;
 space.QuadPart=0;
 if (GetDiskFreeSpaceExA((LPCSTR)disk,&space,NULL,NULL)==FALSE)
 {
  show_error("Can't get the disk free space");
  exit(GET_FREE_SPACE_ERROR);
 }
 return space.QuadPart;
}

size_t write_data(const int target,const unsigned char *buffer,const size_t length)
{
 ptrdiff_t written=0;
 size_t total=0;
 for (total=0;total<length;total+=written)
 {
  written=write(target,buffer+total,length-total);
  if (written<=0)
  {
   total=0;
   break;
  }

 }
 return total;
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
 unsigned char *data=NULL;
 unsigned long long int index=0;
 unsigned long long int elapsed=0;
 size_t written=0;
 size_t block=DATA_BLOCK_LENGTH;
 data=get_memory(block);
 while (index<length)
 {
  elapsed=length-index;
  if (elapsed<=DATA_BLOCK_LENGTH)
  {
   block=(size_t)elapsed;
  }
  written=write_data(target,data,block);
  if (written==0)
  {
   putchar('\n');
   puts("Can't totally wipe the free space");
   break;
  }
  else
  {
   force_write(target,written,DATA_LIMIT);
  }
  index+=block;
  show_progress(index,length);
 }
 free(data);
 putchar('\n');
 puts("Data synchronization in progress. Please wait");
 _commit(target);
 close(target);
}

void do_wipe(const unsigned long int passes,const char drive)
{
 unsigned long int index=0;
 unsigned long long int space=0;
 space=get_free_space(drive);
 puts("Wiping... Please wait");
 for (index=0;index<passes;++index)
 {
  create_temp_directory(drive);
  show_pass(index,passes);
  corrupt_file(create_temp_file(drive),space);
  remove_temp_file(drive);
  remove_temp_directory(drive);
 }
 puts("The wipe was successfully completed");
}

void work(const char *drive,const char *passes)
{
 check_drive(drive);
 do_wipe(get_passes(passes),drive[0]);
}