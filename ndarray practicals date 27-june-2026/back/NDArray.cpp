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
using namespace std;

#define T1 double

class NDArrayException:public std::exception
{
	private:
		std::string message;
	public:
		NDArrayException(const std::string &);
		const char * what() const noexcept override;
};

NDArrayException::NDArrayException(const std::string &_message):message(_message){}
const char * NDArrayException::what() const noexcept
{
	return message.c_str();
}
class NDArrayNode;
class NDArray
{
	private:
		class InfixExpression
		{
			private:
			NDArray *left;
			NDArray *right;
			char oper;
			InfixExpression(NDArray *left,char oper,NDArray *right)
			{
				this->left=left;
				this->right=right;
				this->oper=oper;
			}
			friend class NDArray;
		};
		
		std::vector<T1> collection;
		std::vector<uint32_t> dimensions;
		std::unordered_set<NDArrayNode *> ndArrayNodes;

		void _set(uint32_t,T1);
		T1   _get(uint32_t);
	public:
		vector<uint32_t> get_dimensions()
		{
			return this->dimensions;
		}
	private:
uint32_t getIndex(vector<uint32_t> indexes) const
{
	if(indexes.size()!=this->dimensions.size())
	{
			ostringstream oss;
			oss<<"Index out of bounds ";
			for(auto j:indexes) oss<<"["<<j<<"]";
			oss<<", size of array is ";
			for(auto j:this->dimensions) oss<<"["<<j<<"]";
			throw NDArrayException(oss.str());
	}


	uint64_t idx;
	int x;
	uint32_t multiplier=1;
	idx=0;
	for(x=indexes.size()-1;x>=0;--x)
	{
		if(indexes[x]>=this->dimensions[x])
		{
			ostringstream oss;
			oss<<"Index out of bounds ";
			for(auto j:indexes) oss<<"["<<j<<"]";
			oss<<", size of array is ";
			for(auto j:this->dimensions) oss<<"["<<j<<"]";
			throw NDArrayException(oss.str());
		}
		idx=idx+indexes[x]*multiplier;
		multiplier=multiplier*this->dimensions[x];
	}
	return idx;
}



	public:
		NDArray(const std::vector<uint32_t> &);
		NDArray(const NDArray &);
		NDArray(NDArray &&);
		NDArray & operator=(const NDArray &);
		NDArray & operator=(NDArray &&);
		~NDArray();

		template<typename... TT>
		void set(TT ...);

		template<typename... TT>
		T1 get(TT ...);

		NDArrayNode operator[](uint32_t);
		friend class NDArrayNode;

// do not remove, will be required for copy paste, many many times
void just_do_it(vector<int> &v,vector<int> from,vector<int> to)
{
	int i,k;
	
	int last_index=from.size()-1;
	vector<int> tmp_indices=from;
		while(1)
		{
			while(tmp_indices[last_index]<=to[last_index])
			{
				for(auto x:tmp_indices) cout<<x<<" ";
				cout<<endl;
				tmp_indices[last_index]++;
			}
			k=last_index-1;
			while(k>=0 && tmp_indices[k]==to[k]) k--;
			if(k<0) break;
			tmp_indices[k]++;
			k++;
			while(k<=last_index)
			{
				tmp_indices[k]=from[k];
				k++;
			}
		}
}


InfixExpression operator*(NDArray &right)
{
	return InfixExpression(this,'*',&right);
}
InfixExpression operator/(NDArray &right)
{
	return InfixExpression(this,'/',&right);
}
InfixExpression operator+(NDArray &right)
{
	return InfixExpression(this,'+',&right);
}
InfixExpression operator-(NDArray &right)
{
	return InfixExpression(this,'-',&right);
}
void operator*=(double value)
{
	for(auto &t:this->collection) t*=value;
}
void operator/=(double value)
{
	for(auto &t:this->collection) t/=value;
}
void operator+=(double value)
{
	for(auto &t:this->collection) t+=value;
}
void operator-=(double value)
{
	for(auto &t:this->collection) t-=value;
}

void operator*=(NDArray &other)
{
	NDArray tmp=*this;	
	*this=tmp*other;
}
void operator/=(NDArray &other)
{
	int i=0;
	for(auto &t:this->collection) t/=other.collection[i++];

}
void operator+=(NDArray &other)
{
	int i=0;
	for(auto &t:this->collection) t+=other.collection[i++];
}
void operator-=(NDArray &other)
{
	int i=0;
	for(auto &t:this->collection) t-=other.collection[i++];
}

NDArray operator=(InfixExpression expression)
{
	if(expression.oper=='*') this->multiply(expression);
	else if(expression.oper=='/') this->divide(expression);
	else if(expression.oper=='+') this->add(expression);
	else if(expression.oper=='-') this->subtract(expression);
	return *this;
}
void divide(InfixExpression &expression)
{
int i=0;
for(auto &t:this->collection) 
{
	t=expression.left->collection[i]/expression.right->collection[i];
	++i;
}
}
void add(InfixExpression &expression)
{
int i=0;
for(auto &t:this->collection) 
{
	t=expression.left->collection[i]+expression.right->collection[i];
	++i;
}
}
void subtract(InfixExpression &expression)
{
int i=0;
for(auto &t:this->collection) 
{
	t=expression.left->collection[i]-expression.right->collection[i];
	++i;
}

}

void multiply(InfixExpression &expression)
{
auto left=expression.left;
auto left_array=left->collection.data(); 
auto right=expression.right;
auto right_array=right->collection.data();
uint32_t i;
uint32_t left_matrix_rows;
uint32_t left_matrix_columns;
uint32_t left_matrix_len;
int number_of_left_matrices;
uint32_t right_matrix_rows;
uint32_t right_matrix_columns;
uint32_t right_matrix_len;
int number_of_right_matrices;

if(left->dimensions.size()>=2 && right->dimensions.size()>=2)
{
i=left->dimensions.size();
left_matrix_rows=left->dimensions[i-2];
left_matrix_columns=left->dimensions[i-1];
left_matrix_len=left_matrix_rows*left_matrix_columns;
if(i==2) number_of_left_matrices=1;
else
{
	number_of_left_matrices=1;
	for(int j=0;j<i-2;++j)
	{
		number_of_left_matrices*=left->dimensions[j];
	}
}

i=right->dimensions.size();
right_matrix_rows=right->dimensions[i-2];
right_matrix_columns=right->dimensions[i-1];
right_matrix_len=right_matrix_rows*right_matrix_columns;
if(i==2) number_of_right_matrices=1;
else
{
	number_of_right_matrices=1;
	for(int j=0;j<i-2;++j)
	{
		number_of_right_matrices*=right->dimensions[j];
	}
}
}
else if(left->dimensions.size()==1 && right->dimensions.size()==1)
{
	if(this->dimensions.size()==1 && this->dimensions[0]==1)
	{
		left_matrix_rows=1;
		left_matrix_columns=left->dimensions[0];
		left_matrix_len=left->dimensions[0];
		number_of_left_matrices=1;
		right_matrix_rows=right->dimensions[0];
		right_matrix_columns=1;
		right_matrix_len=right->dimensions[0];
		number_of_right_matrices=1;
	}else if(this->dimensions.size()>=2)
	{
		left_matrix_rows=left->dimensions[0];
		left_matrix_columns=1;
		left_matrix_len=left->dimensions[0];
		number_of_left_matrices=1;
		right_matrix_rows=1;
		right_matrix_columns=right->dimensions[0];
		right_matrix_len=right->dimensions[0];
		number_of_right_matrices=1;
	}else if(left->dimensions[0]==1)
	{
		left_matrix_rows=1;
		left_matrix_columns=1;
		left_matrix_len=1;
		number_of_left_matrices=1;
		right_matrix_rows=1;
		right_matrix_columns=right->dimensions[0];
		right_matrix_len=right->dimensions[0];
		number_of_right_matrices=1;
	}else if(right->dimensions[0]==1)
	{
		left_matrix_rows=left->dimensions[0];
		left_matrix_columns=1;
		left_matrix_len=left->dimensions[0];
		number_of_left_matrices=1;
		right_matrix_rows=1;
		right_matrix_columns=1;
		right_matrix_len=1;
		number_of_right_matrices=1;
	}
}
else if(left->dimensions.size()==1 && right->dimensions.size()>=2)
{
left_matrix_rows=1;
left_matrix_columns=left->dimensions[0];
left_matrix_len=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
i=right->dimensions.size();
right_matrix_rows=right->dimensions[i-2];
right_matrix_columns=right->dimensions[i-1];
right_matrix_len=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}
else if(left->dimensions.size()>=2 && right->dimensions.size()==1)
{
right_matrix_rows=right->dimensions[0];
right_matrix_columns=1;
right_matrix_len=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
i=left->dimensions.size();
left_matrix_rows=left->dimensions[i-2];
left_matrix_columns=left->dimensions[i-1];
left_matrix_len=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
}

auto product_array=this->collection.data();
auto product_matrix_len=left_matrix_rows*right_matrix_columns;

//prefill product_matrix with 0
for(auto &v:this->collection) v=0;
auto product_matrix=product_array;
auto left_matrix=left_array;
auto right_matrix=right_array;

int r,c,c1;

auto cores=thread::hardware_concurrency();

if(number_of_left_matrices>=cores)
{
// omp on outermost loop, think what should be private
}
else
{
// over here, just copy patste, whatever is below

}


for(i=0;i<number_of_left_matrices;++i)
{
#pragma omp parallel for private(c,c1)
for(r=0;r<left_matrix_rows;++r)
{
	for(c=0;c<left_matrix_columns;++c)
	{
		for(c1=0;c1<right_matrix_columns;++c1)
		{
//product_matrix[r][c1]+=(left_matrix[r][c]*right_matrix[c][c1]);
product_matrix[r*right_matrix_columns+c1]+=(left_matrix[r*left_matrix_columns+c]*right_matrix[c*right_matrix_columns+c1]);
		}
	}
}
// implicit barrier
// multiplying 1 matrix ends here
product_matrix+=product_matrix_len;
left_matrix+=left_matrix_len;
right_matrix+=right_matrix_len;
} // multiplying all matrices ends here



} // multiply ends



void print()
{
	int i,k;
	vector<uint32_t> from;
	vector<uint32_t> to;
	for(i=0;i<this->dimensions.size();i++)
	{
		from.push_back(0);
		to.push_back(this->dimensions[i]-1);
	}
	
	int last_index=from.size()-1;
	vector<uint32_t> tmp_indices=from;
		while(1)
		{
			while(tmp_indices[last_index]<=to[last_index])
			{
			for(auto x:tmp_indices) cout<<"["<<x<<"]";
			cout<<"="<<this->collection[this->getIndex(tmp_indices)]<<endl;
			tmp_indices[last_index]++;

			}
			k=last_index-1;
			while(k>=0 && tmp_indices[k]==to[k]) k--;
			if(k<0) break;
			tmp_indices[k]++;
			k++;
			while(k<=last_index)
			{
				tmp_indices[k]=from[k];
				k++;
			}
		}
}


static void fill_random(NDArray &ndArray,vector<uint32_t> from,vector<uint32_t> to,T1 min,T1 max)
{
	int i,k;
	int last_index=from.size()-1;
	vector<uint32_t> tmp_indices=from;
	double value=0;
	random_device rd;
	mt19937 engine(rd());
	normal_distribution<double> dist(0.0,1.0);

		while(1)
		{
			while(tmp_indices[last_index]<=to[last_index])
			{
				value=dist(engine);
				ndArray.collection[ndArray.getIndex(tmp_indices)]=value;
				tmp_indices[last_index]++;
			}
			k=last_index-1;
			while(k>=0 && tmp_indices[k]==to[k]) k--;
			if(k<0) break;
			tmp_indices[k]++;
			k++;
			while(k<=last_index)
			{
				tmp_indices[k]=from[k];
				k++;
			}
		}
}



static void fill(NDArray &ndArray,vector<uint32_t> from,vector<uint32_t> to,T1 value)
{
	int i,k;
	int last_index=from.size()-1;
	vector<uint32_t> tmp_indices=from;
		while(1)
		{
			while(tmp_indices[last_index]<=to[last_index])
			{
				ndArray.collection[ndArray.getIndex(tmp_indices)]=value;
				tmp_indices[last_index]++;
			}
			k=last_index-1;
			while(k>=0 && tmp_indices[k]==to[k]) k--;
			if(k<0) break;
			tmp_indices[k]++;
			k++;
			while(k<=last_index)
			{
				tmp_indices[k]=from[k];
				k++;
			}
		}
}

static void copy(NDArray &target,vector<uint32_t> target_from,const NDArray &source,vector<uint32_t> source_from,vector<uint32_t> source_to)
{
// validation (not doing right now), not computing target_to
	int i,k;
	T1 value; 
	int last_index=source_from.size()-1;
	int offset=target.dimensions.size()-source.dimensions.size();
	vector<uint32_t> source_tmp_indices=source_from;
	vector<uint32_t> target_tmp_indices=target_from;
		while(1)
		{
			while(source_tmp_indices[last_index]<=source_to[last_index])
			{
				value=source.collection[source.getIndex(source_tmp_indices)];
				target.collection[target.getIndex(target_tmp_indices)]=value;
				source_tmp_indices[last_index]++;
				target_tmp_indices[last_index+offset]++; 
			}
			k=last_index-1;
			while(k>=0 && source_tmp_indices[k]==source_to[k]) k--;
			if(k<0) break;
			source_tmp_indices[k]++;
			target_tmp_indices[k+offset]++;
			k++;
			while(k<=last_index)
			{
				source_tmp_indices[k]=source_from[k];
				target_tmp_indices[k+offset]=target_from[k+offset];
				k++;
			}
		}

}

};

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


NDArray::NDArray(const std::vector<uint32_t> &_dimensions)
{

	if(_dimensions.size()==0) throw NDArrayException("NDArray cannot be of zero dimensions");
	int product=std::accumulate(_dimensions.begin(),_dimensions.end(),1,std::multiplies<uint32_t>());
	if(product==0)
	{
		ostringstream oss;
		oss<<"Size of a dimension cannot be zero ";
		for(uint32_t x:_dimensions) oss<<"["<<x<<"]";
		throw NDArrayException(oss.str());
	}
	this->dimensions=_dimensions;
	this->collection.resize(product);
} 


NDArray::NDArray(const NDArray &other)  // copy constructor
{
this->collection=other.collection;
this->dimensions=other.dimensions;
}
NDArray::NDArray(NDArray &&other)  // move constructor
{
this->collection=std::move(other.collection);
this->dimensions=std::move(other.dimensions);
for(NDArrayNode *n:other.ndArrayNodes) 
{
	n->ndArray=NULL;
	n->indexes.resize(0);
}
other.ndArrayNodes.clear();
}

NDArray::~NDArray()	// destructor
{
for(NDArrayNode *n:this->ndArrayNodes) 
{
	n->ndArray=NULL;
	n->indexes.resize(0);
}
}


NDArray & NDArray::operator=(const NDArray &other) // copy assignment operator
{
	for(NDArrayNode *n:this->ndArrayNodes)
	{
		n->ndArray=NULL;
		n->indexes.resize(0);
	}
	ndArrayNodes.clear();
	this->collection=other.collection;
	this->dimensions=other.dimensions;
	return *this;
}
NDArray & NDArray::operator=(NDArray &&other) // move assignment operator
{
	for(NDArrayNode *n:this->ndArrayNodes)
	{
		n->ndArray=NULL;
		n->indexes.resize(0);
	}
	ndArrayNodes.clear();
this->collection=std::move(other.collection);
this->dimensions=std::move(other.dimensions);
for(NDArrayNode *n:other.ndArrayNodes) 
{
	n->ndArray=NULL;
	n->indexes.resize(0);
}
other.ndArrayNodes.clear();
	return *this;
}



void NDArray::_set(uint32_t index,T1 value)
{
	this->collection[index]=value;
}
T1 NDArray::_get(uint32_t index)
{
	return this->collection[index];
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

NDArrayNode NDArray::operator[](uint32_t index)
{
	NDArrayNode node(this,{index});
	return node;		
}

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
int main_whatever()
{
	try
	{
		
	NDArray x({2,4,3});
	NDArray y({2,3,5});
	NDArray z({2,4,5});
	int r,c;
	int j;
	double value;
	for(j=0;j<2;++j)
	{
	value=1.0;
	for(r=0;r<4;++r)
	{
		for(c=0;c<3;++c)
		{
			x.set(j,r,c,value);
			value++;
		}
	}
	}
	for(j=0;j<2;++j)
	{
	value=10.0;
	for(r=0;r<3;++r)
	{
		for(c=0;c<5;++c)
		{
			y.set(j,r,c,value);
			value++;
		}
	}
	}
	z=x*y;
	z.print();

	}catch(const NDArrayException &e)
	{
		cout<<e.what()<<endl;
	}
	return 0;
}


