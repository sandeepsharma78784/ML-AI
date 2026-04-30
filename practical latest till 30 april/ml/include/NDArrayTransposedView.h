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
	InfixExpression<NDArrayTransposedView,NDArray> operator*(NDArray &right);
	friend class NDArray;	
};

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

