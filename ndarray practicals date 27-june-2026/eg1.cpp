#include<iostream>
using namespace std;
class aaa
{
int x;
public:
aaa(int x)
{
this->x=x;
}
aaa()
{
this->x=0;
}
aaa operator-()
{
cout<<"- got called"<<endl;
return aaa(-x);
}
void print()
{
cout<<x<<endl;
}
};
int main()
{
aaa a(50);
aaa b;
b=-a;
b.print();
return 0;
}
