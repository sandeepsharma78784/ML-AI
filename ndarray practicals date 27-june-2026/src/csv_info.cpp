#include<csv_info.h>
#include<string>
#include<cstdint>
using namespace std;
csv_info::csv_info(uint32_t _rows,uint32_t _columns,uint32_t _skip_lines,string _name)
	{
		this->_rows=_rows;
		this->_columns=_columns;
		this->_skip_lines=_skip_lines;
		this->_name=_name;
	}
uint32_t csv_info::rows()
	{
		return this->_rows;
	}
	uint32_t csv_info::columns()
	{
		return this->_columns;
	}
	uint32_t csv_info::header_lines_count()
	{
		return _skip_lines;
	}
	string csv_info::name()
	{
		return this->_name;
	}



