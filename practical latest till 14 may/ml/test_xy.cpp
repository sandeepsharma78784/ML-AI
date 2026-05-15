#include<ml_utils.h>
#include<NDArray.h>
#include<iostream>
using namespace std;
int main()
{
	csv_info csv=ml_utils::get_csv_info("ttt.csv",2);
	cout<<csv.rows()<<endl;
	cout<<csv.columns()<<endl;
	cout<<csv.name()<<endl;
	cout<<csv.header_lines_count()<<endl;
	NDArray x({csv.rows(),csv.columns()});
	NDArray y({csv.rows()});
	ml_utils::get_x_y(csv,x,y);
	x.print();
	cout<<"--------------"<<endl;
	y.print();

	return 0;
}
