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
	NDArray n({4,3});
	int i,j;
	double value=1.0;
	for(i=0;i<4;++i)
	{
		for(j=0;j<3;++j)
		{
			n[i][j]=value++;
		}
	}
	cout<<"n matrix"<<endl;
	print_2d_matrix(n);
	NDArrayTransposedView nt=n.get_transposed_view(true);
	cout<<"n^T matrix"<<endl;
	print_2d_matrix(nt);
	NDArray product({3,3});
	product=nt*n;	
	print_2d_matrix(product);
	
	return 0;
}
