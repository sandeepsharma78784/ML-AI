#pragma once
#include<stdexcept>
#include<string>
using namespace std;

class NDArrayException:public logic_error
{
	public:
		NDArrayException(const std::string &);
};



