/*
in this we will try to overload [] operator with multiple parameters.
to compile this code we will use below command:
    g++ eg1.cpp -std=c++23
*/

#include<iostream>
using namespace std;

class aaa{
    int faltu;
    public :
        int & operator[](int e, int f,int g){ // too many parameters in function call, expected 1
            cout<<e<<" "<<f<<" "<<g<<endl;  
            return faltu;
        }
    
};
int main()
{
    
    aaa a;
    //a[1][2][3]=20;  //  error: no match for 'operator[]', hum soche ki esa kaam krega to ye nahi krega.
    a[1,2,3]=20; // a[1][2][3] is not possible because we have only one operator[] function defined which takes three parameters.
    return 0;
}

/*
Note: In C++, the operator[] can only take one parameter.
If you want to use multiple parameters, 
you can use a different operator or a member function. 
The code above will not compile because it tries to overload the operator[] 
with multiple parameters, which is not allowed in C++.


but this is possible in c++ 23, in c++ 23 we can overload operator[] with 
multiple parameters using the new feature called "operator[] with multiple parameters".

so to compile this code we will use below command:
    g++ eg1.cpp -std=c++23

*/


/*

iska fayda humare liye:
    humare liye NDArray me humne [][][] ko chaining me kiya he.
    abhi is wale se kr skte he a[1,2,3]=20;  // isme humne 3 parameters diye he operator[] ko.
    but humare case me issue ye he ki operator overload kese krne 
    kyuki ndarray to kitne bhi dimension ka bna skta he user.
    to ndarray bnte time user fix number of dimension nahi dega, to vo 
    kbhi 3d kbhi 2d kbhi 4d bna skta he, to hum operator[] ko overload kese kre.

    to humko sochna he ki kya variable number of parameters ke sath operator[] ko overload kr skte he, 
    to c++ 23 me ye possible he.

    means [] ko c++23 style me varargs k sath overload kr skte he kya.
    pr isme bhi sochna he ki varargs wali kahani he uske piche dynamic memory allocation wali kahani he.
    means user:
        a[1,2,3]
        a[2,3,4,5]
        a[2,3,4,5,5,6]

    to hum kya multiple [] opeartor function likhe?
    ya ek hi operator[] function likhe jisme variable number of parameters ho.
    jo upar wale saare cases k liye chle.
    

*/