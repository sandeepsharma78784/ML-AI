#include<ml_functions.h>
#include<NDArray.h>
#include<iostream>
using namespace std;
int main()
{
NDArray n1({3,6,5});
NDArray n2({3,4,3});
double value;
for(uint32_t i=0;i<3;++i)
{
value=1.0;
for(uint32_t r=0;r<6;++r)
{
for(uint32_t c=0;c<5;++c)
{
n1[i][r][c]=value++;
}
}
}
NDArrayView n1view=n1.get_view({0,2,1},{2,4,4},true);
NDArrayTransposedView n2tv=n2.get_transposed_view(true);
n2tv=sigmoid(n1view);
for(uint32_t i=0;i<3;++i)
{
value=1.0;
for(uint32_t r=0;r<3;++r)
{
for(uint32_t c=0;c<4;++c)
{
cout<<n2tv.get(i,r,c)<<" ";
}
cout<<endl;
}
}
cout<<"------------------------------"<<endl;
n2.print();



return 0;
}
