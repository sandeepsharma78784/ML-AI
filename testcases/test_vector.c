#include<sss.h>
#include<stdio.h>
int main()
{
	sss_vector *vector;
	sss_err *error;
	error=sss_error_create_new();
	if(error==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	char error_message[4096];
	vector=sss_vector_create_new(5,error);
	sss_on_error_throw(error,golu);
	sss_vector_set(vector,0,10.23,error);
	sss_vector_set(vector,1,14.23,error);
	sss_vector_set(vector,2,15.23,error);
	sss_vector_set(vector,3,13.23,error);
	sss_vector_set(vector,4,16.23,error);

	sss_vector_write(vector,"vec.data",error);
	sss_on_error_throw(error,golu);
	
	//now destroy the create new wala vector and get one from read vector
	sss_vector_destroy(vector,error);
	vector=sss_vector_read("vec.data",error);
	sss_on_error_throw(error,golu);

	//finally print vector and destroy error and vector and close the test
	sss_vector_print(stdout,vector,error);
	sss_vector_destroy(vector,error);
	sss_error_destroy(error);
	return 0;

golu:
	//print the error thrown
	sss_get_error(error_message,4096,error);
	printf("Error : %s\n",error_message);
	sss_vector_destroy(vector,error);
	sss_error_destroy(error);
}