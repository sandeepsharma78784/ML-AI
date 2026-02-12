// purpose simple example of operator overloading
// ek class he aaa uske andr 3 size ka array 
// usko initialize krna using initializer list and parameterized constructor and 
// aaa a; ek object bnana and us object k satth a[[0] a[1]a[2] esa operation possible krwana


#include<iostream>
using namespace std;
class aaa
{
	private:
		int x[3];
	public:
		aaa(int a,int b,int c): x{a,b,c}{} // parameterized constructor
		void printIt()
		{
		for(int i=0;i<=2;i++) cout<<x[i]<<endl;
		}
		int & operator[](int index)
		{
			return x[index];
		}
};

int main()
{
	aaa a(10,38,19);
	//a.printIt();
	cout<<a[0]<<" "<<a[1]<<" "<<a[2]<<" "<<endl;	
	a[1]=229;
	cout<<a[0]<<" "<<a[1]<<" "<<a[2]<<" "<<endl;	
}

// issue with this code:
	// we are returning a reference to a private member x
	// this can lead to unintended modifications
	// one way to fix this is to return a copy instead
	// int operator[](int index) const
	// but we also need to make sure the array is not modified
	// we can do this by making the operator[] const
	// const int operator[](int index) const
	// But if we do so a[1] = 229; will not be allowed
	// one way to fix this is to provide a non-const version of operator[]
	// int & operator[](int index)
	// This will allow us to modify the array elements
	// but we are loosing encapsulation here
	// another way to fix this is to provide a setter method
	// void set(int index, int value)
	// and a getter method	
	// int get(int index) const
	// This will allow us to control how the array elements are accessed and modified
	// but user want this a[] syntax
	// so we need to use operator overloading but also make sure to preserve encapsulation without any setter getter
	// one way to do this is to return a proxy object from operator[]
	// this proxy object will have a reference to the original object and the index
	// and will provide a way to access and modify the array elements
	// but this is a bit more complex and out of scope for this example