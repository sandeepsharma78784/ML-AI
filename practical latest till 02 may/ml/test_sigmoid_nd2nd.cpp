#include<ml_functions.h>
#include<NDArray.h>
#include<iostream>
using namespace std;
int main()
{
NDArray n1({3,4,6});
NDArray n2({3,4,6});
double value;
value=1.0;
for(uint32_t i=0;i<3;++i)
{
for(uint32_t r=0;r<4;++r)
{
for(uint32_t c=0;c<6;++c)
{
n1[i][r][c]=value++;
}
}
value=(i*10)+100;
}
n2=sigmoid(n1);
n2.print();
return 0;
}
