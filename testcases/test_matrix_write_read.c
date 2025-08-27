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
	matrix=sss_matrix_create_new(4,3,error);
	if(sss_has_error(error))
	{
		sss_get_error(error_message,101,error);
		printf("%s\n",error_message);
		sss_error_destroy(error);
		return 0;
	}
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

	//sss_matrix_print(stdout,matrix,error);
	//if(sss_has_error(error)

	//lets save matrix data to a file
	sss_matrix_write(matrix,"matrix.data",error);
	sss_matrix_destroy(matrix,error);

	//lets read data from file into matrix
	matrix=sss_matrix_read("matrix.data",error);
	
	//print matrix
	sss_matrix_print(stdout,matrix,error);

	sss_matrix_destroy(matrix,error);
	sss_error_destroy(error);

	//below code is to read binary and write actual text using fprintf
	FILE *f1,*f2;
	f1=fopen("matrix.data","rb");
	f2=fopen("abc.data","w");
	uint32_t data;
	double data1;
	fread(&data,sizeof(uint32_t),1,f1);
	fprintf(f2,"%u",data);
	fread(&data,sizeof(uint32_t),1,f1);
	fprintf(f2,"%u",data);
	fread(&data1,sizeof(double),1,f1);
	fprintf(f2,"%12.6lf",data1);
	fclose(f1);
	fclose(f2);
	return 0;

}