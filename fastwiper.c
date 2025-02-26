#include "fastwiper.h"

void show_intro();
void show_help();
void show_pass(const unsigned long int pass,const unsigned long int total);
void show_progress(const unsigned long long int start,const unsigned long long int end);
void check_argument(const char *target);
void check_drive(const char *drive);
unsigned long int get_pass(const char *target);
char *get_memory(const size_t length);
int create_temp_file(const char drive);
unsigned long long int get_wiping_size(const char drive);
void corrupt_file(const int target,const unsigned long long int length);
void remove_temp_file(const char drive);
void create_temp_directory(const char drive);
void remove_temp_directory(const char drive);
void do_wipe(const unsigned long int passes,const char drive);
void work(const char *passes,const char *drive);

int main(int argc, char *argv[])
{
 show_intro();
 switch (argc)
 {
  case 3:
  work(argv[1],argv[2]);
  break;
  case 2:
  work("1",argv[1]);
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
 puts("Version 1.1.4");
 puts("Free space wiping tool by Popov Evgeniy Alekseyevich, 2016-2025 years");
 puts("This program is distributed under GNU GENERAL PUBLIC LICENSE");
 putchar('\n');
}

void show_help()
{
 puts("You must give the amount of the wipe passes and a drive letter as the command-line arguments!");
 puts("The amount of the wipe passes is an optional argument. It is 1 by default");
}

void show_pass(const unsigned long int pass,const unsigned long int total)
{
 printf("The current wipe pass: %lu The total wipe passes: %lu",pass+1,total);
 putchar('\n');
}

void show_progress(const unsigned long long int start,const unsigned long long int end)
{
 unsigned long long int progress;
 progress=(start+1)*100;
 progress/=end;
 putchar('\r');
 printf("The amount of the processed bytes: %llu from %llu. Progress:%llu%%",start,end,progress);
}

void check_argument(const char *target)
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

}

void check_drive(const char *drive)
{
 char target;
 target='0';
 if (strlen(drive)==1)
 {
  target=drive[0];
 }
 if (isalpha(target)==0)
 {
  puts("Can't decode a command-line argument");
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
  puts("You must give a positive non-zero value as the amount of the wipe passes");
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
 char name[]="a:\\trashdata\\trash.tmp";
 int target;
 name[0]=drive;
 target=open(name,O_WRONLY|O_CREAT|O_BINARY,S_IREAD|S_IWRITE);
 if(target==-1)
 {
  puts("Can't create the temporary file");
  exit(1);
 }
 return target;
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

void corrupt_file(const int target,const unsigned long long int length)
{
 char *data=NULL;
 unsigned long long int index;
 size_t block;
 index=0;
 block=4096;
 data=get_memory(block);
 while (index<length)
 {
  if ((length-index)<=(unsigned long long int)block)
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
 }
 free(data);
 putchar('\n');
 close(target);
}

void remove_temp_file(const char drive)
{
 char name[]="a:\\trashdata\\trash.tmp";
 name[0]=drive;
 if (remove(name)!=0)
 {
  puts("Can't destroy the temporary file");
  exit(3);
 }

}

void create_temp_directory(const char drive)
{
 char target[]="a:\\trashdata";
 target[0]=drive;
 if (mkdir(target)==-1)
 {
  puts("Can't create the temporary directory");
  exit(6);
 }

}

void remove_temp_directory(const char drive)
{
 char target[]="a:\\trashdata";
 target[0]=drive;
 if (rmdir(target)==-1)
 {
  puts("Can't destroy the temporary directory");
  exit(7);
 }

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

void work(const char *passes,const char *drive)
{
 check_drive(drive);
 create_temp_directory(drive[0]);
 do_wipe(get_pass(passes),drive[0]);
 remove_temp_directory(drive[0]);
}