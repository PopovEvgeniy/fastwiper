#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <windows.h>

void show_intro();
void show_help();
void show_start_message();
void show_end_message();
void show_pass(const unsigned long int pass,const unsigned long int total);
void show_progress(const unsigned long long int start,const unsigned long long int end);
void check_argument(const char *target);
unsigned char check_drive_letter(const char *drive);
void check_drive(const char *drive);
unsigned long int get_pass(const char *target);
char *get_memory(const size_t length);
int create_temp_file(const char drive);
unsigned long long int get_wiping_size(const char drive);
void corrupt_file(const int target,const unsigned long long int length);
void remove_temp_file(const char drive);
void wipe_free_space(const char drive);
void do_wipe(const unsigned long int passes,const char drive);
void work(const char *passes,const char *drive);

void show_intro()
{
 putchar('\n');
 puts("FAST WIPER");
 puts("Version 0.8.8");
 puts("Free space wiping tool by Popov Evgeniy Alekseyevich, 2016-2020 years");
 puts("This program distributed under GNU GENERAL PUBLIC LICENSE");
 putchar('\n');
}

void show_help()
{
 puts("You must get amount of wipe pass and drive letter as command line argument!");
}

void show_start_message()
{
 puts("Wiping... Please wait");
}

void show_end_message()
{
 puts("Wipe complete");
}

void show_pass(const unsigned long int pass,const unsigned long int total)
{
 printf("Current wipe pass: %lu Total wipe passes: %lu",pass+1,total);
 putchar('\n');
}

void show_progress(const unsigned long long int start,const unsigned long long int end)
{
 unsigned long long int progress;
 progress=start+1;
 progress*=100;
 progress/=end;
 putchar('\r');
 printf("Amount of processed bytes: %llu from %llu. Progress:%llu%%",start,end,progress);
}

void check_argument(const char *target)
{
 size_t index,length;
 length=strlen(target);
 for (index=0;index<length;++index)
 {
  if (isdigit(target[index])==0)
  {
   puts("Can't decode command line argument");
   exit(4);
  }

 }

}

unsigned char check_drive_letter(const char *drive)
{
 unsigned char result;
 result=1;
 if (strlen(drive)!=1)
 {
  result=0;
 }
 else
 {
  if (isalpha(drive[0])==0) result=0;
 }
 return result;
}

void check_drive(const char *drive)
{
  if (check_drive_letter(drive)==0)
  {
   puts("Can't decode command line argument");
   exit(4);
  }

}

unsigned long int get_pass(const char *target)
{
 unsigned long int pass;
 check_argument(target);
 pass=atol(target);
 if (pass==0)
 {
  puts("You must give positive non-zero value as amount of wipe pass");
  exit(5);
 }
 return pass;
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

int create_temp_file(const char drive)
{
 char name[]="a:\\trash.tmp";
 int target;
 name[0]=drive;
 target=open(name,O_BINARY|O_RDWR|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
 if(target==-1)
 {
  puts("Can't create temporary file");
  exit(1);
 }
 return target;
}

unsigned long long int get_wiping_size(const char drive)
{
 unsigned long long int length;
 char disk[]="a:\\";
 disk[0]=drive;
 if(GetDiskFreeSpaceEx((LPCSTR)disk,NULL,NULL,(PULARGE_INTEGER*)&length)==FALSE) length=0;
 return length;
}

void corrupt_file(const int target,const unsigned long long int length)
{
 const size_t block_length=8388608;
 char *data=NULL;
 unsigned long long int index;
 size_t block;
 index=0;
 block=block_length;
 data=get_memory(block);
 while(index<length)
 {
  if(length-index<=(unsigned long long int)block_length) block=(size_t)length-(size_t)index;
  if(write(target,data,block)==-1)
  {
   putchar('\n');
   puts("Can't totally wipe the free space");
   break;
  }
  index+=(unsigned long long int)block;
  show_progress(index,length);
 }
 free(data);
 putchar('\n');
 close(target);
}

void remove_temp_file(const char drive)
{
 char name[]="a:\\trash.tmp";
 name[0]=drive;
 if (remove(name)==0)
 {
  puts("Temporary file successfully destroyed");
 }
 else
 {
  puts("Can't destroy temporary file");
  exit(3);
 }

}

void wipe_free_space(const char drive)
{
 corrupt_file(create_temp_file(drive),get_wiping_size(drive));
 remove_temp_file(drive);
}

void do_wipe(const unsigned long int passes,const char drive)
{
 unsigned long int index;
 show_start_message();
 for (index=0;index<passes;++index)
 {
  show_pass(index,passes);
  wipe_free_space(drive);
 }
 show_end_message();
}

void work(const char *passes,const char *drive)
{
 check_drive(drive);
 do_wipe(get_pass(passes),drive[0]);
}

int main(int argc, char *argv[])
{
 show_intro();
 if (argc!=3)
 {
  show_help();
 }
 else
 {
  work(argv[1],argv[2]);
 }
 return 0;
}