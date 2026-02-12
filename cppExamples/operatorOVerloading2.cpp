// purpose: last example me hum operator overloading ka use karke array elements ko access aur modify kar rahe the
// lekin humein encapsulation ko bhi dhyan me rakhna hoga
// iske liye kuch krte he:
// jo operator function he aaa class me operator [] vo return krega ek Kalia naam ka Object


//  kalia operator [] (int index)
// {
//  kalia k;
// return k;
// }

// esa krne se aaa k object k liye [] chli and vo kalia return kr rahi he.
// means a[0] ko replace kiya jayega k se. cout<< a[0] become k.
//

#include<iostream>
using namespace std;

class kalia
{
 public:
	int operator() () {
		cout<<"Kalia object accessed"<<endl;
		return 889;
	}
	operator int() {
		return 999;
	}
};


class aaa
{
	private:
		int x[3];
	public:
		aaa(int a,int b,int c): x{} // parameterized constructor
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
}