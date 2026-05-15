#include<ml_functions.h>
#include<NDArray.h>
#include<iostream>
using namespace std;
int main()
{
NDArray n1({3,4,6});
double value;
for(uint32_t i=0;i<3;++i)
{
value=1.0;
for(uint32_t r=0;r<4;++r)
{
for(uint32_t c=0;c<6;++c)
{
n1[i][r][c]=value++;
}
}
}
NDArrayView nv1=n1.get_view({1,2,2},{2,3,5},true);
NDArray n2({2,2,4});
n2=sigmoid(nv1);
n2.print();

return 0;
}
