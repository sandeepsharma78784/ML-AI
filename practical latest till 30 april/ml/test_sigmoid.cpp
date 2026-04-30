#include<NDArray.h>
#include<iostream>
#include<ml_functions.h>
using namespace std;
int main()
{
	NDArray n({5});
	n[0]=3.2;
	n[1]=2.9;
	n[2]=5.0;
	n[3]=0.5;
	n[4]=0.3;
	NDArray k({6});
	k[0]=43;
	NDArrayView kv(&k,{1},{5},false);
	kv=sigmoid(n);
	cout<<kv.get(0)<<endl;
	cout<<kv.get(1)<<endl;
	cout<<kv.get(2)<<endl;
	cout<<kv.get(3)<<endl;
	cout<<kv.get(4)<<endl;
	cout<<"---------------"<<endl;
	k.print();


	return 0;
}
