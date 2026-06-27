#pragma once
#include<NDArray.h>
#include<iostream>
#include<InfixExpression.h>

using namespace std;

class NDArrayView 
{
	private:
	NDArray *ndArray;
	bool is_read_only;
	vector<uint32_t> dimensions;
	vector<uint32_t> from;
	vector<uint32_t> to;
	uint32_t start_at;
	vector<uint32_t> base_indices;
	public:
	NDArrayView(NDArray *ndArray,vector<uint32_t> from,vector<uint32_t> to,bool is_read_only);
	vector<uint32_t> get_dimensions();
	template<typename... TT>
	T1 get(TT ...);
	template<typename... TT>
	void set(TT ...);

	template<typename T>
	NDArrayView & operator=(const OneParamFunc<T> &one_param_func);
	NDArrayView & operator*=(double value);

	friend class NDArray;	
	friend class NDArrayTransposedView;
};

template<typename T>
NDArrayView & NDArrayView::operator=(const OneParamFunc<T> &one_param_func)
{
if constexpr(is_same_v<T,NDArray>)
{
	size_t sz=this->to.size();
	uint32_t how_many_elements_in_one_array=this->to[sz-1]-this->from[sz-1]+1;
	uint32_t how_many_arrays=this->base_indices.size();
	int i,j;
	i=0;	
	j=0;
	for(auto e:one_param_func.obj.collection)
	{
		this->ndArray->collection[this->base_indices[i]+j]=one_param_func.func(e);
		j++;
		if(j==how_many_elements_in_one_array)
		{
			j=0;
			i++;
		}
	}
}
if constexpr(is_same_v<T,NDArrayView>)
{
	size_t sz=this->to.size();
	uint32_t how_many_elements_in_one_array=this->to[sz-1]-this->from[sz-1]+1;
	uint32_t how_many_arrays=this->base_indices.size();
	int i,j;
	i=0;	
	j=0;
	double e;
	auto obj=one_param_func.obj;
	while(i<this->base_indices.size())
	{
		e=obj.ndArray->collection[obj.base_indices[i]+j];
		this->ndArray->collection[this->base_indices[i]+j]=one_param_func.func(e);
		j++;
		if(j==how_many_elements_in_one_array)
		{
			j=0;
			i++;
		}
	}
}
if constexpr(is_same_v<T,NDArrayTransposedView>)
{
// this has to be analyzed in details
auto obj=one_param_func.obj;
size_t sz=obj.ndArray->dimensions.size();
if(sz==1)
{
uint32_t i=0;
for(auto x:one_param_func.obj.ndArray->collection)
{
	this->ndArray->collection[i++]=one_param_func.func(x);
}
}
else
{
int number_of_matrices;
auto obj=one_param_func.obj;
number_of_matrices=1;
for(int i=sz-3;i>=0;--i)
{
number_of_matrices*=this->dimensions[i];
}
uint32_t r,c;
size_t rows,columns;
rows=obj.ndArray->dimensions[sz-1];
columns=obj.ndArray->dimensions[sz-2];
int number_of_elements_in_one_matrix=rows*columns;
double *right_ptr=obj.ndArray->collection.data();
double *left_ptr=this->ndArray->collection.data();
uint32_t i=0;
for(int j=1;j<=number_of_matrices;++j)
{
for(r=0;r<rows;++r)
{
for(c=0;c<columns;++c)
{
left_ptr[r*columns+c]=one_param_func.func(right_ptr[c*rows+r]);
}
}
left_ptr+=number_of_elements_in_one_matrix;
right_ptr+=number_of_elements_in_one_matrix;
}
} // else ends
}
return *this;
}

template<typename... TT>
T1 NDArrayView::get(TT ...arguments)
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

	vector<uint32_t> ndarray_indexes=this->from;
	uint32_t i;
	i=this->start_at;
	uint32_t j;
	for(j=0;j<this->dimensions.size();++j)
	{
		ndarray_indexes[i]+=indexes[j];
		++i;
	}
	uint64_t idx;
	uint32_t multiplier=1;
	idx=0;
	for(x=ndarray_indexes.size()-1;x>=0;--x)
	{
		idx=idx+ndarray_indexes[x]*multiplier;
		multiplier=multiplier*this->ndArray->dimensions[x];

	}
	return this->ndArray->_get(idx);
}
template<typename... TT>
void NDArrayView::set(TT ...arguments)
{
	auto dimensions=this->get_dimensions();
	vector<uint32_t> indexes;
	(indexes.push_back((uint32_t)arguments),...);
	indexes.pop_back();
	double value;
	((value=(double)arguments),...);
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

	vector<uint32_t> ndarray_indexes=this->from;
	uint32_t i;
	i=this->start_at;
	uint32_t j;
	for(j=0;j<this->dimensions.size();++j)
	{
		ndarray_indexes[i]+=indexes[j];
		++i;
	}
	uint64_t idx;
	uint32_t multiplier=1;
	idx=0;
	for(x=ndarray_indexes.size()-1;x>=0;--x)
	{
		idx=idx+ndarray_indexes[x]*multiplier;
		multiplier=multiplier*this->ndArray->dimensions[x];

	}
	return this->ndArray->_set(idx,value);
}


