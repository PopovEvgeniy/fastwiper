#include "fastwiper.h"
#include "exitcode.h"
#include "settings.h"

void show_intro();
void show_progress(const unsigned long long int start,const unsigned long long int end);
void show_message(const char *message);
void show_error(const char *message);
void show_system_error(const char *message,const int code);
int create_temp_file(const char drive);
unsigned long long int get_position(const int target);
unsigned char *get_memory(const size_t length);
void remove_temp_file(const char drive);
void check_drive(const char *drive);
void create_temp_directory(const char drive);
void remove_temp_directory(const char drive);
unsigned long long int get_free_space(const char drive);
size_t write_data(const int target,const unsigned char *buffer,const size_t length);
void sync_data(const int target);
void force_write(const int target,const size_t block,const size_t limit);
void fill_zero_bytes(const int target,const unsigned long long int length);
void work(const char *drive);

int main(int argc, char *argv[])
{
 show_intro();
 switch (argc)
 {
  case 1:
  puts("You must give a drive letter as the command-line argument!");
  exit(COMMAND_LINE_ARGUMENTS_ERROR);
  break;
  case 2:
  work(argv[1]);
  break;
  default:
  show_error("You gave too many command-line arguments");
  exit(COMMAND_LINE_ARGUMENTS_ERROR);
  break;
 }
 return 0;
}

void show_intro()
{
 putchar('\n');
 puts("FAST WIPER 1.6.8");
 puts("The free space wiping tool by Popov Evgeniy Alekseyevich, 2016-2026 years");
 puts("This program is distributed under the GNU GENERAL PUBLIC LICENSE (version 2 or later) terms");
 putchar('\n');
}

void show_progress(const unsigned long long int start,const unsigned long long int end)
{
 putchar('\r');
 printf("The amount of the processed bytes: %llu from %llu. The progress:%llu%%",start,end,(start*100)/end);
 fflush(stdout);
}

void show_message(const char *message)
{
 putchar('\n');
 puts(message);
}

void show_error(const char *message)
{
 fputc('\n',stderr);
 fputs(message,stderr);
 fputc('\n',stderr);
}

void show_system_error(const char *message,const int code)
{
 show_error(message);
 fputs(strerror(code),stderr);
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
  show_system_error("Can't create the temporary file",errno);
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
  show_system_error("Can't destroy the temporary file",errno);
  exit(DESTROY_FILE_ERROR);
 }

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
  show_error("The drive letter is invalid");
  exit(INVALID_DRIVE_ERROR);
 }

}

void create_temp_directory(const char drive)
{
 char target[]=TRASH_DIRECTORY;
 target[0]=drive;
 if (mkdir(target)==-1)
 {
  if (errno!=EEXIST)
  {
   show_system_error("Can't create the temporary directory",errno);
   exit(CREATE_DIRECTORY_ERROR);
  }

 }

}

unsigned long long int get_position(const int target)
{
 long long int position=-1;
 position=_lseeki64(target,0,SEEK_CUR);
 if (position==-1)
 {
  show_system_error("Can't get the current offset",errno);
  exit(GET_FILE_POSITION_ERROR);
 }
 return position;
}

void remove_temp_directory(const char drive)
{
 char target[]=TRASH_DIRECTORY;
 target[0]=drive;
 if (rmdir(target)==-1)
 {
  show_system_error("Can't destroy the temporary directory",errno);
  exit(DESTROY_DIRECTORY_ERROR);
 }

}

unsigned long long int get_free_space(const char drive)
{
 char disk[]=DEFAULT_DRIVE;
 ULARGE_INTEGER space;
 disk[0]=drive;
 space.QuadPart=0;
 if (GetDiskFreeSpaceExA((LPCSTR)disk,NULL,NULL,&space)==FALSE)
 {
  show_error("Can't get the disk free space");
  exit(GET_FREE_SPACE_ERROR);
 }
 if (space.QuadPart==0)
 {
  show_error("The target disk is full");
  exit(FULL_DISK_ERROR);
 }
 return space.QuadPart;
}

size_t write_data(const int target,const unsigned char *buffer,const size_t length)
{
 ptrdiff_t written=0;
 size_t total=0;
 while (total<length)
 {
  written=write(target,buffer+total,length-total);
  if (written>0)
  {
   total+=written;
  }
  if (written==0)
  {
   show_system_error("The number of written bytes is invalid: 0",errno);
   exit(WRITE_ZERO_BYTES_ERROR);
  }
  if (written==-1)
  {
   if (errno==ENOSPC)
   {
    show_message("No space left");
    errno=0;
    total=0;
    break;
   }
   else
   {
    show_system_error("Can't write data",errno);
    exit(WRITE_DATA_ERROR);
   }

  }

 }
 return total;
}

void sync_data(const int target)
{
 if (_commit(target)==SYNCHRONIZATION_ERROR)
 {
  show_system_error("Can't synchronize data",errno);
  exit(SYNC_DATA_ERROR);
 }

}

void force_write(const int target,const size_t block,const size_t limit)
{
 static size_t written=0;
 written+=block;
 if (written>=limit)
 {
  sync_data(target);
  written=0;
 }

}

void fill_zero_bytes(const int target,const unsigned long long int length)
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
  if (elapsed<DATA_BLOCK_LENGTH)
  {
   block=(size_t)elapsed;
  }
  written=write_data(target,data,block);
  if (written==0)
  {
   break;
  }
  else
  {
   force_write(target,written,DATA_LIMIT);
   index=get_position(target);
   show_progress(index,length);
  }

 }
 free(data);
 sync_data(target);
 close(target);
}

void work(const char *drive)
{
 int target=-1;
 unsigned long long int space=0;
 check_drive(drive);
 create_temp_directory(drive[0]);
 target=create_temp_file(drive[0]);
 space=get_free_space(drive[0]);
 fill_zero_bytes(target,space);
 remove_temp_file(drive[0]);
 remove_temp_directory(drive[0]);
 show_message("The process is complete");
}