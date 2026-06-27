#include<ml_functions.h>
#include<NDArray.h>
#include<iostream>
using namespace std;
int main()
{
NDArray n1({3,6,4});
NDArray n2({3,4,6});
double value;
for(uint32_t i=0;i<3;++i)
{
value=1.0;
for(uint32_t r=0;r<6;++r)
{
for(uint32_t c=0;c<4;++c)
{
n1[i][r][c]=value++;
}
}
}
NDArrayTransposedView n2tv=n2.get_transposed_view(true);
n2tv=sigmoid(n1);

for(uint32_t i=0;i<3;++i)
{
for(uint32_t r=0;r<6;++r)
{
for(uint32_t c=0;c<4;++c)
{
cout<<n2tv.get(i,r,c)<<" ";
}
cout<<endl;
}
cout<<"------------------------"<<endl;
}



return 0;
}
