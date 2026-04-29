#include<NDArray.h>
#include<NDArrayTransposedView.h>
#include<csv_info.h>
#include<ml_utils.h>
int main()
{	
csv_info  csv=ml_utils::get_csv_info("IceCreamSales.csv",1);
NDArray x({csv.rows(),csv.columns()});
NDArray y({csv.rows()});
auto x_dimensions=x.get_dimensions();
auto x_rows=x_dimensions[0];
auto x_columns=x_dimensions[1];
auto y_dimensions=y.get_dimensions();
auto y_rows=y_dimensions[0];
auto y_columns=y_dimensions[1];
NDArray y_hat({y_rows});
NDArray difference({y_rows});
NDArray squared_sum({1});
NDArray error({1});
ml_utils::get_x_y(csv,x,y);
// x.transpose();   Transpose the contents of x
// NDArray x_transposed=x.as_transposed(); // transpose and return new NDArray
// with transposed form of x

NDArrayTransposedView x_transposed=x.get_transposed_view(true); // true for is_read_only
// the above line may be      rr=x.get_transposed_view();

double learning_rate=0.00001;
NDArray model({x_columns});
NDArray tmp_for_updation({x_columns});
NDArray::fill_random(model,{0},{x_columns-1},0,1);
uint64_t number_of_iterations=1000000;
uint64_t k;

for(k=1;k<=number_of_iterations;++k)
{
y_hat=x*model;
difference=y_hat-y;  // - to be overloaded
squared_sum=difference*difference;
error=squared_sum/(2*x_rows); // to be overloaded
cout<<"Error : "<<error[0]<<endl;
tmp_for_updation=x_transposed*difference;
tmp_for_updation/=x_rows;
tmp_for_updation*=learning_rate;
model-=tmp_for_updation;
}


	return 0;
}
