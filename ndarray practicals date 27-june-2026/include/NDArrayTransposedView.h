#pragma once
#include<NDArray.h>
#include<iostream>
#include<InfixExpression.h>

using namespace std;

class NDArrayTransposedView 
{
	private:
	NDArray *ndArray;
	bool is_read_only;
	public:
	NDArrayTransposedView(NDArray *ndArray,bool is_read_only);
	vector<uint32_t> get_dimensions();
	template<typename... TT>
	T1 get(TT ...);
	InfixExpression<NDArrayTransposedView *,NDArray *> operator*(NDArray &right);
	template<typename T>
	NDArrayTransposedView & operator=(const OneParamFunc<T> &one_param_func);

	friend class NDArray;	
};
template<typename T>
NDArrayTransposedView & NDArrayTransposedView::operator=(const OneParamFunc<T> &one_param_func)
{
if constexpr(is_same_v<T,NDArray>)
{
		auto obj=one_param_func.obj;
		if(obj.dimensions.size()==1)
		{
		uint32_t idx=0;
		for(auto e:obj.collection)
		{
		this->ndArray->collection[idx]=one_param_func.func(e);
		}
		}
		else
		{
		size_t sz=obj.dimensions.size();
		uint32_t number_of_2d_arrays;
		int e=sz-2;
		int i;
		for(i=0,number_of_2d_arrays=1;i<e;++i)
		{
		number_of_2d_arrays=number_of_2d_arrays*obj.dimensions[i];
		}
		double *left_ptr=this->ndArray->collection.data();
		double *right_ptr=obj.collection.data();
		uint32_t left_r,left_c;
		uint32_t right_r,right_c;
		auto right_rows=obj.dimensions[sz-2];
		auto right_columns=obj.dimensions[sz-1];
		auto left_columns=this->ndArray->dimensions[sz-1];

		for(int j=0;j<number_of_2d_arrays;++j)
		{
		for(right_r=0;right_r<right_rows;++right_r)
		{
			for(right_c=0;right_c<right_columns;++right_c)
			{
left_ptr[right_c*left_columns+right_r]=one_param_func.func(right_ptr[right_r*right_columns+right_c]);
			}
		}
		left_ptr+=(right_rows*right_columns);
		right_ptr+=(right_rows*right_columns);
		}
		}
}
if constexpr(is_same_v<T,NDArrayView>)
{
auto obj=one_param_func.obj;
if(obj.base_indices.size()==1)  // view represents horizontal 1D vertical
{
uint32_t i=obj.base_indices[0];
for(auto &e:this->ndArray->collection)
{
e=one_param_func.func(obj.ndArray->collection[i++]);
}
return *this;
}
auto sz=obj.from.size();
size_t size_of_array=obj.to[sz-1]-obj.from[sz-1]+1;
int i,k;
i=0;
k=0;
double value;
int m;
auto size_of_collection=this->ndArray->collection.size();
m=0;
uint32_t r,c;
r=0;
c=0;
auto rows=this->ndArray->dimensions[sz-1]; // as per transposed view
auto columns=this->ndArray->dimensions[sz-2]; // as per transposed view
auto ptr=this->ndArray->collection.data();
while(m<size_of_collection)
{
value=one_param_func.func(obj.ndArray->collection[obj.base_indices[i]+k]);
ptr[c*rows+r]=value;
c++;
if(c==columns)
{
r++;
c=0;
if(r==rows)
{
r=0;
ptr+=(rows*columns);
}
}



m++;
k++;
if(k==size_of_array) 
{
i++;
k=0;
}
}
}

if constexpr(is_same_v<T,NDArrayTransposedView>)
{
double *left_ptr=this->ndArray->collection.data();
auto right_collection=one_param_func.obj.ndArray->collection;
for(auto val:right_collection)
{
	*left_ptr=one_param_func.func(val);
	++left_ptr;
}
}




return *this;
}

template<typename... TT>
T1 NDArrayTransposedView::get(TT ...arguments)
{
	auto dimensions=this->get_dimensions();
	vector<uint32_t> indexes;
	(indexes.push_back((uint32_t)arguments),...);
	if(indexes.size()!=dimensions.size())
	{
	ostringstream oss;

	if(dimensions.size()>1)
	{
	oss<<"Array has "<<dimensions.size()<<" dimensions, expected "<<dimensions.size()<<" indexes, found "<<indexes.size()<<endl;
	}
	else
	{
	oss<<"Array has "<<dimensions.size()<<" dimension, expected "<<dimensions.size()<<" index, found "<<indexes.size()<<endl;
	}

throw NDArrayException(oss.str());
	}
	int x;


	
	for(x=indexes.size()-1;x>=0;--x)
	{
		if(indexes[x]>=dimensions[x])
		{
	ostringstream oss;


	oss<<"Index out of bounds ";
	for(auto j:indexes) oss<<"["<<j<<"]";
	oss<<", size of array is ";
	for(auto j:dimensions) oss<<"["<<j<<"]";

	throw NDArrayException(oss.str());
		}
		
	}

	if(dimensions.size()>1)
	{
		auto sz=dimensions.size();
		auto r=dimensions[sz-2];
		auto c=dimensions[sz-1];
		dimensions[sz-2]=c;
		dimensions[sz-1]=r;
		// dimension reset to original
		r=indexes[sz-2];
		c=indexes[sz-1];
		indexes[sz-2]=c;
		indexes[sz-1]=r;
	}
	uint64_t idx;
	uint32_t multiplier=1;
	idx=0;
	for(x=indexes.size()-1;x>=0;--x)
	{
		idx=idx+indexes[x]*multiplier;
		multiplier=multiplier*dimensions[x];

	}

	return this->ndArray->_get(idx);
}

