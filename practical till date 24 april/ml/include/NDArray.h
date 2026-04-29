#pragma once
#include<omp.h>
#include<vector>
#include<iostream>
#include<stdint.h>
#include<numeric>
#include<unordered_set>
#include<sstream>
#include<thread>
#include<random>
#include<InfixExpression.h>
#include<NDArrayException.h>
#include<NDArrayNode.h>
#include<OneParamFunc.h>

using namespace std;

#define T1 double
class NDArrayNode; // important (forward declaration)
class NDArrayTransposedView; // important (forward declaration)
class NDArrayView;

class NDArray
{
	private:
		
		std::vector<T1> collection;
		std::vector<uint32_t> dimensions;
		std::unordered_set<NDArrayNode *> ndArrayNodes;
		std::unordered_set<NDArrayTransposedView *> ndArrayTransposedViews;

		void _set(uint32_t,T1);
		T1   _get(uint32_t);
	uint32_t getIndex(vector<uint32_t> indexes) const;
	public:
		NDArray(const std::vector<uint32_t> &);
		NDArray(const NDArray &);
		NDArray(NDArray &&);
		NDArray & operator=(const NDArray &);
		NDArray & operator=(NDArray &&);
		~NDArray();
		void to_binary(const string &file_name);
		template<typename... TT>
		void set(TT ...);

		template<typename... TT>
		T1 get(TT ...);

		NDArrayNode operator[](uint32_t);
		vector<uint32_t> get_dimensions();
InfixExpression<NDArray,NDArray> operator*(NDArray &right);
InfixExpression<NDArray,NDArray> operator/(NDArray &right);
InfixExpression<NDArray,NDArray> operator+(NDArray &right);
InfixExpression<NDArray,NDArray> operator-(NDArray &right);

NDArray operator/(double value);
NDArray operator*(double value);
NDArray operator+(double value);
NDArray operator-(double value);

void operator*=(double value);
void operator/=(double value);
void operator+=(double value);
void operator-=(double value);
void operator*=(NDArray &other);
void operator/=(NDArray &other);
void operator+=(NDArray &other);
void operator-=(NDArray &other);
NDArray operator=(InfixExpression<NDArray,NDArray> expression);
NDArray operator=(InfixExpression<NDArrayTransposedView,NDArray> expression);

void divide(InfixExpression<NDArray,NDArray> &expression);
void add(InfixExpression<NDArray,NDArray> &expression);
void subtract(InfixExpression<NDArray,NDArray> &expression);
void multiply(InfixExpression<NDArray,NDArray> &expression);
void multiply(InfixExpression<NDArrayTransposedView,NDArray> &expression);

void print();
static void fill_random(NDArray &ndArray,vector<uint32_t> from,vector<uint32_t> to,T1 min,T1 max);
static void fill(NDArray &ndArray,vector<uint32_t> from,vector<uint32_t> to,T1 value);
static void copy(NDArray &target,vector<uint32_t> target_from,const NDArray &source,vector<uint32_t> source_from,vector<uint32_t> source_to);
	friend class NDArrayNode;
	friend class NDArrayTransposedView;
	friend class NDArrayView;
NDArrayTransposedView get_transposed_view(bool is_read_only);	
NDArrayView get_view(vector<uint32_t> from,vector<uint32_t> to,bool is_read_only);

template<typename T>
NDArray & operator=(const OneParamFunc<T> &one_param_func);
};

template<typename T>
NDArray & NDArray::operator=(const OneParamFunc<T> &one_param_func)
{
/*
 * check if capacity of this->collection>=one_param_func.obj.collection.size()
 * if not then resize this->collection
 * if yes then check if this->collection.size()==one_param_func.obj.collection.size()
 * if not then resize this->collection
 */

uint32_t i=0;
for(auto x:one_param_func.obj.collection)
{
	this->collection[i++]=one_param_func.func(x);
}


return *this;
}




template<typename... TT>
void NDArray::set(TT ...arguments)
{
	vector<uint32_t> indexes;
	double value;

	(indexes.push_back((uint32_t)arguments),...);
	((value=arguments),...);
	
	indexes.pop_back();
	if(indexes.size()!=this->dimensions.size())
	{
	ostringstream oss;
	if(this->dimensions.size()>1)
	{
	oss<<"Array has "<<this->dimensions.size()<<" dimensions, expected "<<this->dimensions.size()<<" indexes, found "<<indexes.size()<<endl;
	}
	else
	{
	oss<<"Array has "<<this->dimensions.size()<<" dimension, expected "<<this->dimensions.size()<<" index, found "<<indexes.size()<<endl;
	}
	throw NDArrayException(oss.str());
	}
	uint64_t idx;
	int x;
	uint32_t multiplier=1;
	idx=0;
	for(x=indexes.size()-1;x>=0;--x)
	{
		if(indexes[x]>=dimensions[x])
		{
	ostringstream oss;
	oss<<"Index out of bounds ";
	for(auto j:indexes) oss<<"["<<j<<"]";
	oss<<", size of array is ";
	for(auto j:this->dimensions) oss<<"["<<j<<"]";

	throw NDArrayException(oss.str());
		}
		idx=idx+indexes[x]*multiplier;
		multiplier=multiplier*dimensions[x];
	}
	this->collection[idx]=value;
}

template<typename... TT>
T1 NDArray::get(TT ...arguments)
{
	vector<uint32_t> indexes;
	(indexes.push_back((uint32_t)arguments),...);
	if(indexes.size()!=this->dimensions.size())
	{
	ostringstream oss;

	if(this->dimensions.size()>1)
	{
	oss<<"Array has "<<this->dimensions.size()<<" dimensions, expected "<<this->dimensions.size()<<" indexes, found "<<indexes.size()<<endl;
	}
	else
	{
	oss<<"Array has "<<this->dimensions.size()<<" dimension, expected "<<this->dimensions.size()<<" index, found "<<indexes.size()<<endl;
	}

throw NDArrayException(oss.str());
	}
	uint64_t idx;
	int x;
	uint32_t multiplier=1;
	idx=0;
	for(x=indexes.size()-1;x>=0;--x)
	{
		if(indexes[x]>=dimensions[x])
		{
	ostringstream oss;
	oss<<"Index out of bounds ";
	for(auto j:indexes) oss<<"["<<j<<"]";
	oss<<", size of array is ";
	for(auto j:this->dimensions) oss<<"["<<j<<"]";

	throw NDArrayException(oss.str());
		}
		idx=idx+indexes[x]*multiplier;
		multiplier=multiplier*dimensions[x];
	}
	return this->collection[idx];
}

