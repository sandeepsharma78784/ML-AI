#pragma once
#include<string>
#include<cstdint>
using namespace std;
class csv_info
{
	uint32_t  _rows;
	uint32_t  _columns;
	uint32_t  _skip_lines;
	string _name;
	public:
	csv_info(uint32_t _rows,uint32_t _columns,uint32_t _skip_lines,string _name);
	uint32_t rows();
	uint32_t columns();
	uint32_t header_lines_count();
	string name();

};

