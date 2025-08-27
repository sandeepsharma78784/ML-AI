#include<sss.h>
#include<stdio.h>

int main()
{
	char error_message[101];
	uint32_t r,c;
	double value;
	uint32_t rows,columns;
	sss_matrix *matrix;
	sss_err *error;
	error=sss_error_create_new();
	if(error==NULL)
	{
	printf("Out of memory\n");
	return 0;	
	}
	matrix=sss_matrix_read_csv("abcd.csv",0,error);
	if(sss_has_error(error))
	{
	printf("something get wrong\n");
	sss_get_error(error_message,101,error);
	printf("Error [%s]\n",error_message);
	//return 0;
	}
	sss_matrix_print(stdout,matrix,error);
	sss_matrix_destroy(matrix,error);
	sss_error_destroy(error);

return 0;
}