#include<stdio.h>
#include<math.h>
#include<stdlib.h>
int main(int c, char *vv[])
{
if(c!=3)
{
printf("requires program_name numerator denominator\n");
return 0;
}

int a=atol(vv[1]);
int b=atol(vv[2]);

if(b==0)
{
printf("can not compute\n");
return 12;
}
printf("result a/b is =  %d\n",a/b);
}