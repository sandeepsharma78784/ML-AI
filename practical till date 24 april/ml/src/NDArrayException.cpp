#include<NDArrayException.h>
#include<string>
using namespace std;

NDArrayException::NDArrayException(const std::string &_message):logic_error("NDArrayException : "+_message){}


