#pragma once
#include<NDArray.h>
#include<NDArrayView.h>
#include<NDArrayTransposedView.h>
#include<OneParamFunc.h>

using namespace std;

double sigmoid(double x);

template<typename T,
	typename=enable_if_t<disjunction_v<is_same<T,NDArray>,is_same<T,NDArrayTransposedView>,is_same<T,NDArrayView>>>>
OneParamFunc<T> sigmoid(T &obj)
{
	return OneParamFunc<T>(obj,sigmoid);
}


