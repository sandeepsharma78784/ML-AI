#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
int x,y,n,m,c;
int min,max;
srand(time(0));
FILE *f;
f=fopen("line.data","w");
m=1;
c=1;
n=1;
x=1;
while(n<=100)
{
y=m*x+c;
//fprintf(f,"%d,%d\n",x,y);
fprintf(f,"%d,%d\n",x+(rand()%(max-min)+min),y+(rand()%(max-min)+min));
x++;
n++;
}
fclose(f);
return 0;
}