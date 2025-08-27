#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
int x;
int min,max;
min=-10;
max=5;
srand(time(0));
for(x=1;x<=10;x++)
{
printf("%d\n",rand()%(max-min) + min);
}
return 0;
}