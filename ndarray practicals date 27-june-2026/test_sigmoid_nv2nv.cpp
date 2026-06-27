#include<ml_functions.h>
#include<iostream>
#include<NDArray.h>
#include<NDArrayView.h>
int main()
{
NDArray nd1({3,5,5});
double value=1.0;
for(int i=0;i<3;++i)
{
for(int r=0;r<5;++r)
{
for(int c=0;c<5;++c)
{
nd1[i][r][c]=value++;
}
}
}
NDArrayView nv1=nd1.get_view({1,1,1},{2,3,4},true);

NDArray nd2({3,10,10});
value=100.0;
for(int i=0;i<3;++i)
{
for(int r=0;r<10;++r)
{
for(int c=0;c<10;++c)
{
nd2[i][r][c]=value++;
}
}
}
NDArrayView nv2=nd2.get_view({0,3,3},{1,5,6},true);
nv2=sigmoid(nv1);
nd1.print();
nd2.print();
return 0;
}
