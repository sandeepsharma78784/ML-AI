#include<stdio.h>
#include<stdlib.h>

int main(int c, char *vv[])
{
if(c!=3)
{
printf("requires program_name numerator denominator\n");
return 0;
}
FILE *f;
char buffer[128];
char command[256];
char result[4096];

//create a command 
sprintf(command, "compute_division.out %s %s",vv[1],vv[2]);


//open other  program
f=popen(command,"r");


if(f==NULL)
{
perror("popen failed");
return 1;
}

//if we reach here means program gets executed
// collect result
while(fgets(buffer,sizeof(buffer),f)!=NULL)
{
sprintf(result, "%s", buffer);  // or we can write it to a file
}


// check the program exit code status
int exit_status_code=pclose(f);

if(exit_status_code==12) 
{
printf("Exit code of program is %d\n",exit_status_code);
printf("cannot divide\n");
}
else
{
printf("Result: %s", result);  // or we can write it to a file
}



return 0;
}