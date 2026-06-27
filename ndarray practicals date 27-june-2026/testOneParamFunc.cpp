#include<OneParamFunc.h>
#include<NDArray.h>
#include<NDArrayView.h>
#include<NDArrayTransposedView.h>
class aaa
{
};
double sss(double t)
{
	return t;
}
int main()
{
aaa aa;
NDArray n({10,10});
OneParamFunc<NDArray> a(n,sss);
//OneParamFunc<aaa> b(aa,sss);
//OneParamFunc<NDArrayTransposedView> b;
//OneParamFunc<NDArrayTransposedView> c;

};
