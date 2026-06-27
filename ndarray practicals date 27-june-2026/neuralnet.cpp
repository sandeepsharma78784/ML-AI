#include<NDArray.cpp>
#include<iostream>
#include<ml_utils.h>
#include<csv_info.h>

int main()
{
// known fact : 2D
// classification problem
csv_info  csv=ml_utils::get_csv_info("digits.csv",0);
NDArray x({csv.rows(),csv.columns()});
NDArray y({csv.rows(),1});
auto x_dimensions=x.get_dimensions();
auto x_rows=x_dimensions[0];
auto x_columns=x_dimensions[1];
auto y_dimensions=y.get_dimensions();
auto y_rows=y_dimensions[0];
auto y_columns=y_dimensions[1];

ml_utils::get_x_y(csv,x,y); // call by ref
// now we have input features as (x) bias included
// now we have ouput as (y)
uint32_t number_of_layers=5;
uint32_t number_of_classes=10; // ???????? from where this info has been extracted
vector<NDArray *> nu(number_of_layers);
vector<NDArray *> v(number_of_layers-1);
vector<NDArray *> model(number_of_layers-1);

vector<uint32_t> nu_dimensions={x_columns,4,6,3,number_of_classes};
vector<uint32_t> v_dimensions(number_of_layers-1);

int i;
for(i=0;i<v_dimensions.size()-1;++i) v_dimensions[i]=nu_dimensions[i+1]-1;
v_dimensions[i]=nu_dimensions[i+1];

for(i=0;i<nu_dimensions.size();++i) nu[i]=new NDArray({nu_dimensions[i]});
for(i=0;i<v_dimensions.size();++i) v[i]=new NDArray({v_dimensions[i]});
for(i=0;i<number_of_layers-1;++i) 
{
model[i]=new NDArray({v_dimensions[i],nu_dimensions[i]});
NDArray::fill_random(*model[i],{0,0},{v_dimensions[i]-1,nu_dimensions[i]-1},0,1);
}


// release all
for(auto &t:nu) delete t;
for(auto &t:v) delete t;
for(auto &t:model) delete t;

	return 0;
}
