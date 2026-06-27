#pragma once
#include<NDArray.h>
#include<NDArrayView.h>
#include<NDArrayTransposedView.h>
#include<OneParamFunc.h>
#include<math.h>

using namespace std;

double sigmoid(double x);
double der_sigmoid(double x);

template<typename T,
	typename=enable_if_t<disjunction_v<is_same<T,NDArray>,is_same<T,NDArrayTransposedView>,is_same<T,NDArrayView>>>>
OneParamFunc<T> log(T &obj)
{
	return OneParamFunc<T>(obj,log);
}

template<typename T,
	typename=enable_if_t<disjunction_v<is_same<T,NDArray>,is_same<T,NDArrayTransposedView>,is_same<T,NDArrayView>>>>
OneParamFunc<T> sigmoid(T &obj)
{
	return OneParamFunc<T>(obj,sigmoid);
}

template<typename T,
	typename=enable_if_t<disjunction_v<is_same<T,NDArray>,is_same<T,NDArrayTransposedView>,is_same<T,NDArrayView>>>>
OneParamFunc<T> der_sigmoid(T &obj)
{
	return OneParamFunc<T>(obj,sigmoid);
}


template<typename TT1,typename TT2>
InfixExpression<TT1 *,TT2 *> pointwise_product(TT1 &left,TT2 &right)
{
	return InfixExpression<TT1 *,TT2 *>(&left,'.',&right);

}


template<typename TT>
InfixExpression<double,TT *> operator-(double left,TT &right)
{
	return InfixExpression<double,TT *>(left,'-',&right);

}

