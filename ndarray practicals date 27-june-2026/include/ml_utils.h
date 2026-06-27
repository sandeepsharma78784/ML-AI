#pragma once
#include<NDArray.h>
#include<csv_info.h>
#include<fstream>
#include<stdexcept>
#include<sstream>
using namespace std;
class ml_utils
{
	public:
	static csv_info get_csv_info(string name,uint32_t skip_lines);
	static void get_x_y(csv_info csv,NDArray &x,NDArray &y);
};

