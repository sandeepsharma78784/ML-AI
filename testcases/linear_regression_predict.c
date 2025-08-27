#include<sss.h>
#include<stdio.h>
#include<stdlib.h>  
#include<private_sss.h> // ye hata dege to seterror k liye compile time error aayga

sss_vector * sss_linear_regression_predict(const sss_matrix *x_matrix, sss_vector *model_vector, sss_err *error)
{
	// x is input and y is output , data spliteed from data set file
 sss_matrix *X_matrix=NULL;
 sss_vector *Y_HAT_vector=NULL;

 
 char error_message[4096];
 uint32_t x_matrix_rows,x_matrix_columns;
 uint32_t model_vector_size;

if(error) sss_clear_error(error);
if(x_matrix==NULL)
{
	sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
	goto catch_error;
}
if(model_vector==NULL)
{
	sss_set_error(error,"Null pointer argument(2)",SSS_NULL_POINTER);
	goto catch_error;
}
sss_matrix_get_dimensions(x_matrix,&x_matrix_rows,&x_matrix_columns,error);
model_vector_size=sss_vector_get_size(model_vector,error);


if(model_vector_size!=(x_matrix_columns+1)) // why we compared with rows because x ko transpose krege to rows will converted to col
{
 sprintf(error_message,"invalid model size (%u) required (%u)",model_vector_size,x_matrix_columns+1);
 sss_set_error(error,error_message,SSS_INVALID_MODEL_SIZE);
 goto catch_error;
}

// yaha tak sahi
X_matrix=sss_matrix_create_new(x_matrix_rows,x_matrix_columns+1,error); // dataset se naya input vector banaya with one column filled with 1
if(X_matrix==NULL) goto catch_error; // error matrix creation wale ne set kr di hogi

// fill the first col of input matrix
sss_matrix_fill(X_matrix,0,0,x_matrix_rows-1,0,1.0,NULL); //error ki jagah null pass kiya

// now copy the input from x to X for (0,1) to (n-1,n-1)
sss_matrix_copy(X_matrix,0,1, x_matrix,0,0,x_matrix_rows-1,x_matrix_columns-1,NULL);


Y_HAT_vector=sss_multiply_matrix_with_vector(X_matrix,model_vector,NULL,error);
if(Y_HAT_vector==NULL) goto catch_error;



goto prepare_for_exit;
    catch_error:
		// nothing to do , or we can print the error;
    prepare_for_exit:
        sss_matrix_destroy(X_matrix, NULL);

return Y_HAT_vector;
}


int main(int c,  char *v[])
{
char error_message[4096];
sss_err *error;

sss_matrix *dataset=NULL;  //read from csv
uint32_t dataset_rows,dataset_columns;

sss_vector *model=NULL;  //actual model

sss_matrix *x=NULL;  //read from csv
uint32_t x_rows,x_columns;

sss_vector *y=NULL;  //actual model
sss_vector *y_hat=NULL;  //actual model



double r2score;

char * dataset_filename;
char * model_filename;


if(c!=3)
{
	printf("pass dataset_name model_name\n");
}
dataset_filename =  v[1];
model_filename =  v[2];

//init error
error=sss_error_create_new();
if(error==NULL)
{
	printf("Low memory\n");
	return 0;
}


//load dataset into a matrix 
dataset=sss_matrix_read_csv(dataset_filename,0,error);   // skip lines 0
sss_on_error_throw(error,err);  // this macro will check error obj and then decide to go to err if some error
printf("Printing the dataset after reading\n");

model=sss_vector_read_csv(model_filename,0,error);  ///start from here
sss_on_error_throw(error,err);  // this macro will check error obj and then decide to go to err if some error

printf("Printing the model after reading\n");
sss_vector_print(stdout,model,error);


sss_matrix_get_dimensions(dataset,&dataset_rows,&dataset_columns,error);
x=sss_matrix_create_new(dataset_rows,dataset_columns-1,error);  // vo y vector bnane wala coloumn hata diya
sss_on_error_throw(error,err);  // this macro will check error obj and then decide to go to err if some error
sss_matrix_get_dimensions(x,&x_rows,&x_columns,error);

// ab copy kr do 
sss_matrix_copy(x,0,0, dataset,0,0,dataset_rows-1,dataset_columns-2,NULL);


// create output vector
y=sss_matrix_get_vector(dataset,0,dataset_columns-1,dataset_rows-1,dataset_columns-1,error);
sss_matrix_destroy(dataset,error);


// now get the final y hat from liear regression predict()
y_hat=sss_linear_regression_predict(x,model,error);  // fit ne jo model banaya the vo model he ye
sss_on_error_throw(error,err);  // this macro will check error obj and then decide to go to err if some error


r2score=sss_r2_score(y,y_hat,error);
printf("R2 Score %lf\n",r2score);
	goto exit_program;

err:
	sss_get_error(error_message,4096,error);
	printf("Error : %s\n",error_message);
exit_program:
		// destroy wala kaam, jaha bhi memory alocaiton ka kaaam hua
		sss_matrix_destroy(dataset,error);
		sss_matrix_destroy(x,error);
		sss_vector_destroy(y,error);
		sss_vector_destroy(y_hat,error);
		sss_vector_destroy(model,error);
		sss_error_destroy(error);

	return 0;
}