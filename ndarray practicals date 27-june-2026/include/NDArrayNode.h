#pragma once
#include<iostream>
#include<vector>
#include<NDArray.h>
#define T1 double
class NDArray; // important, forward declaration
class NDArrayNode
{
	private:
		NDArray *ndArray;
		vector<uint32_t> indexes;
		NDArrayNode(NDArray *,const vector<uint32_t> &indexes);		
		uint32_t getIndex() const;
	public:
		NDArrayNode operator[](uint32_t);
		operator T1();
		T1 operator=(T1);
		~NDArrayNode();
	
		NDArrayNode(const NDArrayNode &);
		NDArrayNode(NDArrayNode &&);
		NDArrayNode & operator=(const NDArrayNode &);
		NDArrayNode & operator=(NDArrayNode &&);
		friend class NDArray;
};


