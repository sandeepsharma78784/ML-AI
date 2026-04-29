#pragma once
#include<type_traits>
using namespace std;
class NDArray;
class NDArrayTransposedView;
class NDArrayView;
template<typename T,
	typename=enable_if_t<disjunction_v<is_same<T,NDArray>,is_same<T,NDArrayTransposedView>,is_same<T,NDArrayView>>>>
class OneParamFunc
{
	public:
		T &obj;
		double (*func)(double);
OneParamFunc(T &obj,double (*func)(double)):obj(obj),func(func){}
};
