#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <windows.h>

void show_intro();
void show_start_message();
void show_end_message();
void show_pass(unsigned long int pass,unsigned long int total);
void show_progress(unsigned long long int start,unsigned long long int stop);
void check_argument(char *target);
unsigned long int get_pass(char *target);
char *get_memory(unsigned long int length);
int create_temp_file(char drive);
unsigned long long int get_wiping_size(char drive);
void corrupt_file(int target,unsigned long long int length);
void remove_temp_file(char drive);
void wipe_free_space(char drive);

void show_intro()
{
 printf("\n");
 puts("FAST WIPER");
 puts("Version 0.7");
 puts("Free space wiping tool by Popov Evgeniy Alekseyevich, 2016 year");
 puts("This program distributed under GNU GENERAL PUBLIC LICENSE");
 printf("\n");
}

void show_start_message()
{
 puts("Wiping... Please wait");
}

void show_end_message()
{
 puts("Wipe complete");
}

void show_pass(unsigned long int pass,unsigned long int total)
{
 printf("Current wipe pass: %ld Total wipe passes: %ld",pass+1,total);
 putchar('\n');
}

void show_progress(unsigned long long int start,unsigned long long int end)
{
 unsigned long long int progress;
 progress=start+1;
 progress*=100;
 progress/=end;
 printf("\r");
 printf("Amount of processed bytes: %lld from %lld. Progress:%lld%%",start,end,progress);
}

void check_argument(char *target)
{
 unsigned long int index;
 for (index=strlen(target);index>0;index--)
 {
  if (isdigit(target[index-1])==0)
  {
   puts("Can't decode argument");
   exit(4);
  }

 }

}

unsigned long int get_pass(char *target)
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

char *get_memory(unsigned long int length)
{
 char *memory=NULL;
 memory=(char*)calloc(length,1);
 if(memory==NULL)
 {
  puts("Can't allocate memory");
  exit(2);
 }
 return memory;
}

int create_temp_file(char drive)
{
 char name[13]="a:\\trash.tmp";
 int target;
 name[0]=drive;
 target=open(name,O_BINARY|O_WRONLY|O_CREAT);
 if(target==-1)
 {
  puts("Can't create temporary file");
  exit(1);
 }
 return target;
}

unsigned long long int get_wiping_size(char drive)
{
 unsigned long long int length;
 char disk[4]="a:\\";
 disk[0]=drive;
 if(GetDiskFreeSpaceEx((LPCSTR)disk,NULL,NULL,(PULARGE_INTEGER*)&length)==FALSE) length=0;
 return length;
}

void corrupt_file(int target,unsigned long long int length)
{
 const unsigned long int block_length=8388608;
 char *data=NULL;
 unsigned long long int index;
 unsigned long int block;
 index=0;
 block=block_length;
 data=get_memory(block);
 while(index<length)
 {
  if(length-index<=block_length) block=length-index;
  if(write(target,data,block)==-1)
  {
   printf("\n");
   puts("Can't totally wipe the free space");
   break;
  }
  index+=block;
  show_progress(index,length);
 }
 free(data);
 printf("\n");
 close(target);
}

void remove_temp_file(char drive)
{
 char name[13]="a:\\trash.tmp";
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

void wipe_free_space(char drive)
{
 int target;
 target=create_temp_file(drive);
 corrupt_file(target,get_wiping_size(drive));
 remove_temp_file(drive);
}

int main(int argc, char *argv[])
{
 unsigned long int index,pass;
 show_intro();
 if (argc!=3)
 {
  puts("You must get amount of wipe pass and drive letter as command line argument!");
 }
 else
 {
  if (strlen(argv[2])!=1)
  {
   puts("Can't decode command line argument");
   exit(4);
  }
  else
  {
   if (isalpha(argv[2][0])==0)
   {
    puts("Can't decode command line argument");
    exit(4);
   }
   pass=get_pass(argv[1]);
   show_start_message();
   for (index=0;index<pass;index++)
   {
    show_pass(index,pass);
    wipe_free_space(argv[2][0]);
   }
   show_end_message();
  }

 }
 return 0;
}