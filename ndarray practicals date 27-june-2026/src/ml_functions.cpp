#include<ml_functions.h>
double sigmoid(double x)
{
	return 1.0/(1.0+exp(-x));
}

double der_sigmoid(double x)
{
return sigmoid(x)*(1-sigmoid(x));
}

