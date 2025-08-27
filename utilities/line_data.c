#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
    int x,y,n,m,c;
    int min,max;
    FILE *f;
    srand(time(0));
    f=fopen("line.data","w");
    min=-5;
    max=20;
    m=1; //slope
    c=1;  // intercept where line cuts the y azis
    n=1;
    x=1;
    while(n<=200)    // 100 ki jagah 3 le rahe he for unit testing
    {
        y=m*x+c;
        fprintf(f,"%d,%d\n",x+(rand()%(max-min)+min),y+(rand()%(max-min)+min));
        x++;
        n++;
    }
    fclose(f);
    return 0;
}