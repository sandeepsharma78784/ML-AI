#include<ml_functions.h>
#include<NDArray.h>
#include<NDArrayView.h>
#include<iostream>
using namespace std;
int main()
{
NDArray right_operand({5});
NDArray result({5});
NDArray left_operand({9});
right_operand[0]=1.0;
right_operand[1]=2.0;
right_operand[2]=3.0;
right_operand[3]=4.0;
right_operand[4]=5.0;

left_operand[0]=10.0;
left_operand[1]=11.0;
left_operand[2]=12.0;
left_operand[3]=13.0;
left_operand[4]=14.0;
left_operand[5]=15.0;
left_operand[6]=16.0;
left_operand[7]=17.0;
NDArrayView left_operand_view=left_operand.get_view({2},{6},true);
result=pointwise_product(left_operand_view,right_operand);
result.print();
return 0;
}
