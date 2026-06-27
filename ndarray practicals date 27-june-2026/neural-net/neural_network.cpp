#define TRAINING_CYCLES 100000
#include<NDArray.h>
#include<NDArrayView.h>
#include<NDArrayTransposedView.h>
#include<ml_utils.h>
#include<csv_info.h>
#include<iostream>
#include<ml_functions.h>
using namespace std;
int main()
{
//load dataset
//csv_info csv=ml_utils::get_csv_info("tmdigits.csv",1);
csv_info csv=ml_utils::get_csv_info("tmdigits.csv",1);
double regularization_parameter=0.0001; // to avoid overfitting
double learning_rate=0.0001;
NDArray x({csv.rows(),csv.columns()});
NDArray target_class({csv.rows()});
ml_utils::get_x_y(csv,x,target_class);
// create 2D y from info in target_class
NDArray y({csv.rows(),10});
// populate y by using data from target_class
NDArray::fill(y,{0,0},{csv.rows()-1,9},0);
size_t idx;
for(idx=0;idx<csv.rows();++idx)
{
// each element of target class has a number between (1 to 10)
y[idx][target_class[idx]-1]=1.0;
}
// create 2D y_hat with dimensions same as that of y
NDArray y_hat({csv.rows(),10});
vector<uint32_t> layer_wise_neurons_count={csv.columns(),100,200,10};
vector<NDArray *> layers(layer_wise_neurons_count.size()); 
vector<NDArrayTransposedView *> layersTransposed(layer_wise_neurons_count.size()-1);
NDArray target_y({10});

idx=0;
for(auto cnt:layer_wise_neurons_count) 
{
layers[idx]=new NDArray({cnt});
(*(layers[idx]))[0]=1.0; // ideally not required for first and last layer, but wouldn't matter
idx++;
}
for(idx=0;idx<layer_wise_neurons_count.size()-1;++idx)
{
layersTransposed[idx]=new NDArrayTransposedView(layers[idx],true);
}

vector<NDArray *> z(layers.size()-1);
vector<NDArray *> lower_case_deltas(layers.size()-1);
for(idx=1;idx<layer_wise_neurons_count.size()-1;++idx) 
{
z[idx-1]=new NDArray({layer_wise_neurons_count[idx]-1});
lower_case_deltas[idx-1]=new NDArray({layer_wise_neurons_count[idx]-1});
}
z[idx-1]=new NDArray({layer_wise_neurons_count[idx]});
lower_case_deltas[idx-1]=new NDArray({layer_wise_neurons_count[idx]});
vector<NDArray *> models(layers.size()-1);
vector<NDArray *> tmp_models(layers.size()-1);
vector<NDArrayView *> tmp_model_views(layers.size()-1);

vector<NDArray *> upper_case_deltas(layers.size()-1);
vector<NDArray *> tmp_upper_case_deltas(layers.size()-1);
for(idx=0;idx<z.size()-1;++idx)
{
models[idx]=new NDArray({layer_wise_neurons_count[idx+1]-1,layer_wise_neurons_count[idx]});
tmp_models[idx]=new NDArray({layer_wise_neurons_count[idx+1]-1,layer_wise_neurons_count[idx]});

tmp_model_views[idx]=new NDArrayView(tmp_models[idx],{0,1},{layer_wise_neurons_count[idx+1]-1-1,layer_wise_neurons_count[idx]-1},true);
upper_case_deltas[idx]=new NDArray({layer_wise_neurons_count[idx+1]-1,layer_wise_neurons_count[idx]});
tmp_upper_case_deltas[idx]=new NDArray({layer_wise_neurons_count[idx+1]-1,layer_wise_neurons_count[idx]});
}
models[idx]=new NDArray({layer_wise_neurons_count[idx+1],layer_wise_neurons_count[idx]});
tmp_models[idx]=new NDArray({layer_wise_neurons_count[idx+1],layer_wise_neurons_count[idx]});
tmp_model_views[idx]=new NDArrayView(tmp_models[idx],{0,1},{layer_wise_neurons_count[idx+1]-1,layer_wise_neurons_count[idx]-1},true);
upper_case_deltas[idx]=new NDArray({layer_wise_neurons_count[idx+1],layer_wise_neurons_count[idx]});
tmp_upper_case_deltas[idx]=new NDArray({layer_wise_neurons_count[idx+1],layer_wise_neurons_count[idx]});
vector<NDArrayView *> layers_views(layers.size()-1);
for(idx=0;idx<layers_views.size()-1;++idx)
{
layers_views[idx]=new NDArrayView(layers[idx+1],{1},{layer_wise_neurons_count[idx+1]-1},true);
}
layers_views[idx]=new NDArrayView(layers[idx+1],{0},{layer_wise_neurons_count[idx+1]-1},true);

// fill model with random values between 0.0 and 1.0
for(idx=0;idx<z.size()-1;++idx)
{
NDArray::fill_random(*(models[idx]),{0,0},{layer_wise_neurons_count[idx+1]-2,layer_wise_neurons_count[idx]-1},0.0,1.0);
}
NDArray::fill_random(*(models[idx]),{0,0},{layer_wise_neurons_count[idx+1]-1,layer_wise_neurons_count[idx]-1},0.0,1.0);

uint32_t x_row_index;
auto x_rows=csv.rows();
auto x_columns=csv.columns();
uint64_t ii,jj;
jj=layers.size()-1;
int iii;
int previous_target_y=1;
int current_target_y;
for(iii=0;iii<10;++iii) target_y[iii]=0.0;
vector<NDArrayTransposedView *> model_tvs(lower_case_deltas.size()-1);
vector<NDArray *> tmps(lower_case_deltas.size()-1);
vector<NDArray *> g_dash(lower_case_deltas.size()-1);
vector<NDArrayView *> tmp_views(lower_case_deltas.size()-1);

for(iii=0;iii<lower_case_deltas.size()-1;++iii)
{
model_tvs[iii]=new NDArrayTransposedView(models[iii+1],true);
tmps[iii]=new NDArray({layer_wise_neurons_count[iii+1]});
g_dash[iii]=new NDArray({layer_wise_neurons_count[iii+1]-1});
tmp_views[iii]=new NDArrayView(tmps[iii],{1},{layer_wise_neurons_count[iii+1]-1},true);
}

uint64_t cycle=1;
while(cycle<=TRAINING_CYCLES)
{
// assign 0.0 to upper case deltas before loading the first training example on input layer
for(iii=0;iii<upper_case_deltas.size()-1;++iii)
{
NDArray::fill(*(upper_case_deltas[iii]),{0,0},{layer_wise_neurons_count[iii+1]-2,layer_wise_neurons_count[iii]-1},0.0);
}
NDArray::fill(*(upper_case_deltas[iii]),{0,0},{layer_wise_neurons_count[iii+1]-1,layer_wise_neurons_count[iii]-1},0.0);

// iterate on all training examples
for(x_row_index=0;x_row_index<x_rows;++x_row_index) // after testing, change <1 to <x_rows
{
// pick x_row_index-th row and place it on first layer
NDArray::copy(*(layers[0]),{0},x,{x_row_index,0},{x_row_index,x_columns-1});
target_y[previous_target_y-1]=0.0;
current_target_y=(int)(target_class[x_row_index]-1.0);
target_y[current_target_y]=1.0;
previous_target_y=current_target_y;

for(ii=0;ii<jj;++ii)
{
*(z[ii])=(*(models[ii]))*(*(layers[ii]));
*(layers_views[ii])=sigmoid(*(z[ii]));
}
// update y_hat with info on output_layer
NDArray::copy(y_hat,{x_row_index,0},*(layers[ii-1]),{0},{9});

iii=layers.size()-2;
*lower_case_deltas[iii] = *layers[iii+1] - target_y;
// compute lower_case deltas
for(--iii;iii>=0;--iii)
{
*(tmps[iii]) = *(model_tvs[iii]) * (*(lower_case_deltas[iii+1]));
*(g_dash[iii]) = der_sigmoid(*(z[iii]));
*(lower_case_deltas[iii])=pointwise_product(*(tmp_views[iii]),*(g_dash[iii]));
}
// update upper_case_deltas
for(idx=0;idx<upper_case_deltas.size();++idx)
{
*(tmp_upper_case_deltas[idx])= *(lower_case_deltas[idx])*(*(layersTransposed[idx]));
*(upper_case_deltas[idx])=*(upper_case_deltas[idx])+ (*(tmp_upper_case_deltas[idx]));
}
} // loop on x rows ends here
// something to compute cost
// 1/m summation summation (-y)*log(y_hat) - (1-y)*log(1-y_hat) + reg/2m summ summ summ theta^2
NDArray minus_y({csv.rows(),10});
NDArray log_y_hat({csv.rows(),10});
NDArray one_minus_y({csv.rows(),10});
NDArray one_minus_y_hat({csv.rows(),10});
NDArray log_one_minus_y_hat({csv.rows(),10});
NDArray left_product({csv.rows(),10});
NDArray right_product({csv.rows(),10});
NDArray difference({csv.rows(),10});

minus_y=-y;
log_y_hat=log(y_hat);
one_minus_y=1.0-y;
one_minus_y_hat=1.0-y_hat;
log_one_minus_y_hat=log(one_minus_y_hat);
left_product=pointwise_product(minus_y,log_y_hat);
right_product=pointwise_product(one_minus_y,log_one_minus_y_hat);
difference=left_product-right_product;
double cost=difference.sum()/(double)(csv.rows());

// computing sum theta squared
double models_sum=0.0;
for(idx=0;idx<models.size();++idx)
{
*(tmp_models[idx])=(*(models[idx]))^2;
models_sum+=(*(tmp_models[idx])).sum();
}
cost+=(regularization_parameter/(((double)csv.rows())*2)*models_sum);
printf("Cost %20.10lf\n",cost);
// something to update model
// the common part
for(idx=0;idx<upper_case_deltas.size();++idx)
{
(*(upper_case_deltas[idx]))/=x_rows;
*(tmp_models[idx])=*(models[idx]);
(*(tmp_model_views[idx]))*=(regularization_parameter/x_rows);
(*(upper_case_deltas[idx]))+=(*(tmp_models[idx]));
(*(upper_case_deltas[idx]))*=learning_rate;
(*(models[idx]))-=(*upper_case_deltas[idx]);
}

++cycle;
} // training cycles loop ends


cout<<"Done"<<endl;
// code to clean up / free allocations

return  0;
}

