#include<NDArrayView.h>
NDArrayView::NDArrayView(NDArray *ndArray,vector<uint32_t> from,vector<uint32_t> to,bool is_read_only)
{
	// validations not yet applied
	this->ndArray=ndArray;
	this->from=from;
	this->to=to;
	this->is_read_only=is_read_only;
	int i=0;
	while(i<from.size())
	{
		if(from[i]!=to[i]) break;
		++i;
	}
	size_t from_sz=from.size();
	if(i==from_sz) 
	{
		this->dimensions.resize(1); // one element 1D array
		this->dimensions[0]=1;
		this->start_at=from_sz-1;
	}else 
	{
		size_t sz=this->from.size()-i;
		if(sz==2)
		{
			if(from[from_sz-1]==to[from_sz-1]) // vertical 1D
			{
		this->start_at=from_sz-2;
		this->dimensions.resize(1);
		this->dimensions[0]=to[from_sz-2]-from[from_sz-2]+1;
			}
			else
			{
	this->start_at=from_sz-2;				
	this->dimensions.resize(sz);
	this->dimensions[0]=to[from_sz-2]-from[from_sz-2]+1;
	this->dimensions[1]=to[from_sz-1]-from[from_sz-1]+1;
			}
		}else if(sz==1)
		{
	this->start_at=from_sz-1;
	this->dimensions.resize(1);	
	this->dimensions[0]=to[from_sz-1]-from[from_sz-1]+1;
		}else
		{
	//this->dimensions.resize(sz);
	this->start_at=i;
	while(i<from.size())
	{
		this->dimensions.push_back(to[i]-from[i]+1);
		++i;
	}
		}
	}

// code to compute base_indices
vector<uint32_t> temp;
size_t sz=this->ndArray->dimensions.size();
vector<uint32_t> elements_count(sz);
uint32_t product=1;
for(i=sz-1;i>=0;--i)
{
elements_count[i]=product;
product*=this->ndArray->dimensions[i];
}
int number_of_arrays;
number_of_arrays=1;
for(i=sz-2;i>=0;--i)
{
number_of_arrays*=(to[i]-from[i]+1);
}
this->base_indices.resize(number_of_arrays);
int j;
uint32_t index;
temp=from;
for(j=0;j<number_of_arrays;++j)
{
index=0;
for(i=0;i<sz;++i)
{
index+=(temp[i]*elements_count[i]);
}
this->base_indices[j]=index;
i=temp.size()-2;
while(i>=0)
{
if(temp[i]<to[i])
{
temp[i]++;
break;
}
else
{
temp[i]=from[i];
i--;
}
}
}
}
vector<uint32_t> NDArrayView::get_dimensions()
{
	return this->dimensions;
}
NDArrayView & NDArrayView::operator*=(double value)
{
	size_t sz=this->to.size();
	uint32_t how_many_elements_in_one_array=this->to[sz-1]-this->from[sz-1]+1;
	uint32_t how_many_arrays=this->base_indices.size();
	int i,j;
	i=0;	
	j=0;
	double e;
	while(i<this->base_indices.size())
	{
		this->ndArray->collection[this->base_indices[i]+j]*=value;
		j++;
		if(j==how_many_elements_in_one_array)
		{
			j=0;
			i++;
		}
	}
return *this;
}

