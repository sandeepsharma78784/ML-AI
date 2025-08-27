#include<sss.h>
#include<stdio.h>

int main()
{
	char error_message[101];
	uint32_t r,c;
	double value;
	uint32_t rows,columns;
	sss_matrix *matrix;
	sss_matrix *input_matrix;   //step 1
	sss_err *error;
	error=sss_error_create_new();
	input_matrix = sss_matrix_create_new(4,3,error);
	if(error==NULL)
	{
		printf("Out of memory\n");
		return 0;
	}
	matrix=sss_matrix_create_new(4,3,error);
	sss_on_error_throw(error,err);
	sss_matrix_set(matrix,0,0,10.2,error);
	sss_matrix_set(matrix,0,1,15.2,error);
	sss_matrix_set(matrix,0,2,14.3,error);
	sss_matrix_set(matrix,1,0,1223.55,error);
	sss_matrix_set(matrix,1,1,33.5,error);
	sss_matrix_set(matrix,1,2,55.2,error);
	sss_matrix_set(matrix,2,0,87.6,error);
	sss_matrix_set(matrix,2,1,654.8,error);
	sss_matrix_set(matrix,2,2,43.7,error);
	sss_matrix_set(matrix,3,0,34.9,error);
	sss_matrix_set(matrix,3,1,242.5,error);
	sss_matrix_set(matrix,3,2,44.7,error);

//test filll matrix
printf("test fill matrix\n");
sss_matrix_fill(input_matrix,0,0,3,2,2.0,error);
sss_on_error_throw(error,err);
sss_matrix_print(stdout,matrix,error);
printf("original vs copied\n");
sss_matrix_print(stdout,input_matrix,error);

//test copy matrix
printf("test copy matrix\n");
sss_matrix_copy(input_matrix,0,0,matrix,0,0,3,2,error);
sss_on_error_throw(error,err);
sss_matrix_print(stdout,matrix,error);
printf("original vs copied\n");
sss_matrix_print(stdout,input_matrix,error);

//test get vector from matrix
printf("test get vector from matrix\n");
sss_vector *v=sss_matrix_get_vector(matrix,0,1,3,1,error);
sss_on_error_throw(error,err);
sss_matrix_print(stdout,matrix,error);
printf("original vs copied\n");
sss_vector_print(stdout,v,error);

	sss_matrix_destroy(matrix,error);
	sss_error_destroy(error);
	sss_vector_destroy(v,error)
	return 0;

err:
	sss_get_error(error_message,101,error);
	printf("%s",error_message);
	sss_matrix_destroy(matrix,error);
	sss_error_destroy(error);
	sss_vector_destroy(v,error)
	return 0;


}