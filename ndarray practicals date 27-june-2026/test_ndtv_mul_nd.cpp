#include<NDArray.h>
#include<iomanip> // for setw
#include<iostream>
#include<NDArrayTransposedView.h>
using namespace std;

void print_2d_matrix(NDArrayTransposedView &n)
{
	auto dimensions=n.get_dimensions();
	if(dimensions.size()!=2)
	{
		cout<<"Not a 2 d array"<<endl;
		return;
	}
	auto rows=dimensions[0];
	auto columns=dimensions[1];
	for(int r=0;r<rows;++r)
	{
		for(int c=0;c<columns;++c)
		{
			cout<<setw(10)<<n.get(r,c);
		}
		cout<<endl;
	}
}

void print_2d_matrix(NDArray &n)
{
	auto dimensions=n.get_dimensions();
	if(dimensions.size()!=2)
	{
		cout<<"Not a 2 d array"<<endl;
		return;
	}
	auto rows=dimensions[0];
	auto columns=dimensions[1];
	for(int r=0;r<rows;++r)
	{
		for(int c=0;c<columns;++c)
		{
			cout<<setw(10)<<n[r][c]<<" ";
		}
		cout<<endl;
	}
}

int main()
{
	NDArray n({2,4,3});
	int i,j,k;

	double value;
	for(k=0;k<2;++k)
	{
	value=1.0;
	for(i=0;i<4;++i)
	{
		for(j=0;j<3;++j)
		{
			n[k][i][j]=value++;
		}
	}
	}
	NDArrayTransposedView nt=n.get_transposed_view(true);
	NDArray n2({2,3,3});
	n2=nt*n;

	for(k=0;k<2;++k)
	{
	for(i=0;i<3;++i)
	{
		for(j=0;j<3;++j)
		{
			cout<<n2[k][i][j]<<" ";
		}
		cout<<endl;
	}
cout<<"-----------------"<<endl;
	}



	
	return 0;
}
