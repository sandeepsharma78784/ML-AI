#include<ml_utils.h>
#include<csv_info.h>
#include<fstream>
#include<stdexcept>
#include<sstream>
using namespace std;

csv_info   ml_utils::get_csv_info(string name,uint32_t skip_lines)
{
	ifstream file(name);
	if(file.fail()) throw invalid_argument(string("Unable to open : ")+string(name));
	uint32_t x=1;
	string line;
	while(x<=skip_lines && getline(file,line)) x++;
	if(x<=skip_lines) 
	{
		file.close();
		throw invalid_argument(string("Header does not have : ")+to_string(skip_lines));
	}
	if(!getline(file,line))
	{
		file.close();
		throw invalid_argument(string("File :")+name+string("is not a csv file"));
	}
	stringstream ss(line);
	uint64_t columns=0;
	string str;
	while(getline(ss,str,',')) columns++;
	uint64_t rows=1;
	while(getline(file,line)) rows++;
	file.close();
	return csv_info(rows,columns,skip_lines,name);
}

void ml_utils::get_x_y(csv_info csv,NDArray &x,NDArray &y)
{
	ifstream file(csv.name());
	if(file.fail()) throw invalid_argument(string("Unable to open : ")+string(csv.name()));
	uint32_t i=1;
	string line;
	while(i<=csv.header_lines_count() && getline(file,line)) i++;
	if(i<=csv.header_lines_count()) 
	{
		file.close();
		throw invalid_argument(string("Header does not have : ")+to_string(csv.header_lines_count()));
	}
	string str;
	uint32_t r,c;
	r=0;
	uint32_t last_column_index=csv.columns()-1;
	while(getline(file,line))
	{
		x.set(r,0,1.0);

		c=1;
		stringstream ss(line);
		while(getline(ss,str,','))
		{
			if(c<=last_column_index) x.set(r,c,stod(str));
			else y.set(r,stod(str));
			c++;
		}
		r++;
	}
	file.close();
}

