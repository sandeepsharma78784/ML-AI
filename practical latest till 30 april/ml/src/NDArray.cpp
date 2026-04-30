#include<omp.h>
#include<vector>
#include<cstdio>
#include<iostream>
#include<stdint.h>
#include<numeric>
#include<unordered_set>
#include<sstream>
#include<thread>
#include<random>

#include<NDArray.h>
#include<NDArrayException.h>
#include<NDArrayNode.h>
#include<NDArrayTransposedView.h>
#include<NDArrayView.h>

using namespace std;

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

NDArrayNode NDArray::operator[](uint32_t index)
{
	NDArrayNode node(this,{index});
	return node;		
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


// implementation of NDArray methods start here


InfixExpression<NDArray,NDArray> NDArray::operator*(NDArray &right)
{
	return InfixExpression<NDArray,NDArray>(this,'*',&right);
}
InfixExpression<NDArray,NDArray> NDArray::operator/(NDArray &right)
{
	return InfixExpression<NDArray,NDArray>(this,'/',&right);
}
InfixExpression<NDArray,NDArray> NDArray::operator+(NDArray &right)
{
	return InfixExpression<NDArray,NDArray>(this,'+',&right);
}
InfixExpression<NDArray,NDArray> NDArray::operator-(NDArray &right)
{
	return InfixExpression<NDArray,NDArray>(this,'-',&right);
}
void NDArray::operator*=(double value)
{
	for(auto &t:this->collection) t*=value;
}
void NDArray::operator/=(double value)
{
	for(auto &t:this->collection) t/=value;
}
void NDArray::operator+=(double value)
{
	for(auto &t:this->collection) t+=value;
}
void NDArray::operator-=(double value)
{
	for(auto &t:this->collection) t-=value;
}

void NDArray::operator*=(NDArray &other)
{
	NDArray tmp=*this;	
	*this=tmp*other;
}
void NDArray::operator/=(NDArray &other)
{
	int i=0;
	for(auto &t:this->collection) t/=other.collection[i++];

}
void NDArray::operator+=(NDArray &other)
{
	int i=0;
	for(auto &t:this->collection) t+=other.collection[i++];
}
void NDArray::operator-=(NDArray &other)
{
	int i=0;
	for(auto &t:this->collection) t-=other.collection[i++];
}
NDArray NDArray::operator=(InfixExpression<NDArrayTransposedView,NDArray> expression)
{
	if(expression.oper=='*') this->multiply(expression);
	return *this;
}

NDArray NDArray::operator=(InfixExpression<NDArray,NDArray> expression)
{
	if(expression.oper=='*') this->multiply(expression);
	else if(expression.oper=='/') this->divide(expression);
	else if(expression.oper=='+') this->add(expression);
	else if(expression.oper=='-') this->subtract(expression);
	return *this;
}
void NDArray::divide(InfixExpression<NDArray,NDArray> &expression)
{
int i=0;
for(auto &t:this->collection) 
{
	t=expression.left->collection[i]/expression.right->collection[i];
	++i;
}
}
void NDArray::add(InfixExpression<NDArray,NDArray> &expression)
{
int i=0;
for(auto &t:this->collection) 
{
	t=expression.left->collection[i]+expression.right->collection[i];
	++i;
}
}
void NDArray::subtract(InfixExpression<NDArray,NDArray> &expression)
{
int i=0;
for(auto &t:this->collection) 
{
	t=expression.left->collection[i]-expression.right->collection[i];
	++i;
}

}


void NDArray::multiply(InfixExpression<NDArray,NDArray> &expression)
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



void NDArray::print()
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


void NDArray::fill_random(NDArray &ndArray,vector<uint32_t> from,vector<uint32_t> to,T1 min,T1 max)
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



void NDArray::fill(NDArray &ndArray,vector<uint32_t> from,vector<uint32_t> to,T1 value)
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

void NDArray::copy(NDArray &target,vector<uint32_t> target_from,const NDArray &source,vector<uint32_t> source_from,vector<uint32_t> source_to)
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
		vector<uint32_t> NDArray::get_dimensions()
		{
			return this->dimensions;
		}

uint32_t NDArray::getIndex(vector<uint32_t> indexes) const
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


NDArrayTransposedView NDArray::get_transposed_view(bool is_read_only)
{
	return NDArrayTransposedView(this,is_read_only);
}

void NDArray::multiply(InfixExpression<NDArrayTransposedView,NDArray> &expression)
{
// This is not going to compile and even if it gets compiled, lot of logical changes
// are required
auto left=expression.left;
auto left_array=left->ndArray->collection.data(); 
auto right=expression.right;
auto right_array=right->collection.data();
auto left_dimensions=left->get_dimensions();
uint32_t i;
uint32_t left_matrix_rows;
uint32_t left_matrix_columns;
uint32_t left_matrix_ndarray_columns;
uint32_t left_matrix_len;
int number_of_left_matrices;
uint32_t right_matrix_rows;
uint32_t right_matrix_columns;
uint32_t right_matrix_len;
int number_of_right_matrices;
auto left_matrix_ndarray_dimensions=left->ndArray->get_dimensions();
auto sz=left_matrix_ndarray_dimensions.size();
left_matrix_ndarray_columns=left_matrix_ndarray_dimensions[sz-1];
if(left_dimensions.size()>=2 && right->dimensions.size()>=2)
{
i=left_dimensions.size();
left_matrix_rows=left_dimensions[i-2];
left_matrix_columns=left_dimensions[i-1];
left_matrix_len=left_matrix_rows*left_matrix_columns;
if(i==2) number_of_left_matrices=1;
else
{
	number_of_left_matrices=1;
	for(int j=0;j<i-2;++j)
	{
		number_of_left_matrices*=left_dimensions[j];
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
else if(left_dimensions.size()==1 && right->dimensions.size()==1)
{
	if(this->dimensions.size()==1 && this->dimensions[0]==1)
	{
		left_matrix_rows=1;
		left_matrix_columns=left_dimensions[0];
		left_matrix_len=left_dimensions[0];
		number_of_left_matrices=1;
		right_matrix_rows=right->dimensions[0];
		right_matrix_columns=1;
		right_matrix_len=right->dimensions[0];
		number_of_right_matrices=1;
	}else if(this->dimensions.size()>=2)
	{
		left_matrix_rows=left_dimensions[0];
		left_matrix_columns=1;
		left_matrix_len=left_dimensions[0];
		number_of_left_matrices=1;
		right_matrix_rows=1;
		right_matrix_columns=right->dimensions[0];
		right_matrix_len=right->dimensions[0];
		number_of_right_matrices=1;
	}else if(left_dimensions[0]==1)
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
		left_matrix_rows=left_dimensions[0];
		left_matrix_columns=1;
		left_matrix_len=left_dimensions[0];
		number_of_left_matrices=1;
		right_matrix_rows=1;
		right_matrix_columns=1;
		right_matrix_len=1;
		number_of_right_matrices=1;
	}
}
else if(left_dimensions.size()==1 && right->dimensions.size()>=2)
{
left_matrix_rows=1;
left_matrix_columns=left_dimensions[0];
left_matrix_len=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
i=right->dimensions.size();
right_matrix_rows=right->dimensions[i-2];
right_matrix_columns=right->dimensions[i-1];
right_matrix_len=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}
else if(left_dimensions.size()>=2 && right->dimensions.size()==1)
{
right_matrix_rows=right->dimensions[0];
right_matrix_columns=1;
right_matrix_len=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
i=left_dimensions.size();
left_matrix_rows=left_dimensions[i-2];
left_matrix_columns=left_dimensions[i-1];
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
//product_matrix[r][c1]+=(left_matrix[c][r]*right_matrix[c][c1]);
product_matrix[r*right_matrix_columns+c1]+=(left_matrix[c*left_matrix_ndarray_columns+r]*right_matrix[c*right_matrix_columns+c1]);
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

NDArray NDArray::operator/(double value)
{
	NDArray tmp=*this;
	for(auto &t:tmp.collection) t/=value;
	return tmp;
}
NDArray NDArray::operator*(double value)
{
	NDArray tmp=*this;
	for(auto &t:tmp.collection) t*=value;
	return tmp;
}
NDArray NDArray::operator+(double value)
{
	NDArray tmp=*this;
	for(auto &t:tmp.collection) t+=value;
	return tmp;
}
NDArray NDArray::operator-(double value)
{
	NDArray tmp=*this;
	for(auto &t:tmp.collection) t-=value;
	return tmp;
}
void NDArray::to_binary(const string &file_name)
{
// trim and validate length
FILE *f=fopen(file_name.c_str(),"wb"); 
uint64_t sz=this->dimensions.size();
fwrite(&sz,sizeof(uint64_t),1,f);
fwrite(this->dimensions.data(),sizeof(uint32_t),sz,f);
fwrite(this->collection.data(),sizeof(T1),this->collection.size(),f);
fclose(f);
}

NDArrayView NDArray::get_view(vector<uint32_t> from,vector<uint32_t> to,bool is_read_only)
{
	// validation goes here
	NDArrayView ndArrayView(this,from,to,is_read_only);
	return ndArrayView;
}

