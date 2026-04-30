#include<iostream>
#include<NDArrayNode.h>
#include<NDArray.h>
NDArrayNode::NDArrayNode(NDArray *ndArray,const vector<uint32_t> &indexes)
{
	this->ndArray=ndArray;
	this->indexes=indexes;
	if(this->ndArray) this->ndArray->ndArrayNodes.insert(this);
}


NDArrayNode::NDArrayNode(const NDArrayNode &other)
{
	this->ndArray=other.ndArray;
	this->indexes=other.indexes;
	if(this->ndArray) this->ndArray->ndArrayNodes.insert(this);
}
NDArrayNode::NDArrayNode(NDArrayNode &&other)
{
	this->ndArray=other.ndArray;
	this->indexes=other.indexes;
	if(this->ndArray) this->ndArray->ndArrayNodes.insert(this);
}
NDArrayNode & NDArrayNode::operator=(const NDArrayNode &other)
{ 

if(this->ndArray==NULL) throw NDArrayException(string("Invalid left operand NDArrayNode, array does not exist."));
if(other.ndArray==NULL) throw NDArrayException(string("Invalid right operand NDArrayNode, array does not exist."));
this->ndArray->collection[this->getIndex()]=other.ndArray->collection[other.getIndex()];
return *this;
}

NDArrayNode & NDArrayNode::operator=(NDArrayNode &&other)
{
if(this->ndArray==NULL) throw NDArrayException(string("Invalid left operand NDArrayNode, array does not exist."));
if(other.ndArray==NULL) throw NDArrayException(string("Invalid right operand NDArrayNode, array does not exist."));
this->ndArray->collection[this->getIndex()]=other.ndArray->collection[other.getIndex()];
return *this;
}

NDArrayNode::~NDArrayNode()
{
	if(this->ndArray) this->ndArray->ndArrayNodes.erase(this);
}


NDArrayNode NDArrayNode::operator[](uint32_t index)
{
	if(this->ndArray==NULL) 
	{
		throw NDArrayException(string("Invalid NDArrayNode, array does not exist."));
	}
	uint32_t verify_using_idx=this->indexes.size();
	vector<uint32_t> tmpvec=this->indexes;
	tmpvec.push_back(index);
	NDArrayNode  node(this->ndArray,tmpvec);
	return node;
}  

NDArrayNode::operator T1()
{
	if(this->ndArray==NULL)
	{
		throw NDArrayException(string("Invalid NDArrayNode, array does not exist."));
	}
return this->ndArray->collection[this->getIndex()];
}

T1  NDArrayNode::operator=(T1 value)
{
	
	if(this->ndArray==NULL)
	{
		throw NDArrayException(string("Invalid NDArrayNode, array does not exist."));
	}
	this->ndArray->collection[this->getIndex()]=value;
	return value;	
}
uint32_t NDArrayNode::getIndex() const
{

	if(this->indexes.size()!=this->ndArray->dimensions.size())
	{
			ostringstream oss;
			oss<<"Index out of bounds ";
			for(auto j:this->indexes) oss<<"["<<j<<"]";
			oss<<", size of array is ";
			for(auto j:this->ndArray->dimensions) oss<<"["<<j<<"]";
			throw NDArrayException(oss.str());
	}


	uint64_t idx;
	int x;
	uint32_t multiplier=1;
	idx=0;

	for(x=this->indexes.size()-1;x>=0;--x)
	{
		if(this->indexes[x]>=this->ndArray->dimensions[x])
		{
			ostringstream oss;
			oss<<"Index out of bounds ";
			for(auto j:this->indexes) oss<<"["<<j<<"]";
			oss<<", size of array is ";
			for(auto j:this->ndArray->dimensions) oss<<"["<<j<<"]";
			throw NDArrayException(oss.str());
		}
		idx=idx+this->indexes[x]*multiplier;
		multiplier=multiplier*this->ndArray->dimensions[x];
	}
	return idx;
}

