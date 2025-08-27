#include<sss.h>
#include<stdio.h>
#include<stdlib.h>  // we used atol()
// since we are writng the code as consumer of library to we aare not allowe to use seterror coz it is a private function
// but humne use kiya he to usko idr include to krna pdega
#include<private_sss.h> // ye hata dege to seterror k liye compile time error aayga

// sss_vector * sss_linear_regression_fit(const sss_matrix *x_matrix,const sss_vector *y_vector,uint64_t iterations,double learning_rate, sss_err *error)
sss_vector * sss_linear_regression_fit(const sss_matrix *x_matrix, sss_vector *y_vector,uint64_t iterations,double learning_rate, sss_err *error)
{
	// x is input and y is output , data spliteed from data set file
 sss_matrix *X_matrix=NULL;
 sss_matrix *XT_matrix=NULL;
 sss_vector *Y_HAT_vector=NULL;
 sss_vector *E_vector=NULL;
 sss_vector *ET_vector=NULL;
 sss_vector *ETE_vector=NULL;
 sss_vector *XTE_vector=NULL;
 sss_vector *model_vector=NULL;

 double error_value;
 uint64_t i;
 char error_message[4096];
 uint32_t x_matrix_rows,x_matrix_columns;
 uint32_t X_matrix_rows,X_matrix_columns;

 uint32_t y_vector_size;
 uint32_t model_vector_size;

if(error) sss_clear_error(error);
if(x_matrix==NULL)
{
	sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
	goto catch_error;
}
if(y_vector==NULL)
{
	sss_set_error(error,"Null pointer argument(2)",SSS_NULL_POINTER);
	goto catch_error;
}
if(iterations==0)
{
	sss_set_error(error,"Cannot generate model with 0 iterations",SSS_INVALID_NUMBER_OF_ITERATIONS);
	goto catch_error;
}
sss_matrix_get_dimensions(x_matrix,&x_matrix_rows,&x_matrix_columns,error);



y_vector_size=sss_vector_get_size(y_vector,error);
if(y_vector_size!=x_matrix_rows) // why we compared with rows because x ko transpose krege to rows will converted to col
{
 sprintf(error_message,"y size (%u) does not match x rows (%u)",y_vector_size,x_matrix_rows);
 sss_set_error(error,error_message,SSS_INPUT_AND_TARGET_OUTPUT_SIZE_MISMATCH);
 goto catch_error;
}
X_matrix_rows = x_matrix_rows;  /// here x is dataset and X and y_vector me hum usko split krege
X_matrix_columns= x_matrix_columns;
X_matrix=sss_matrix_create_new(X_matrix_rows,X_matrix_columns,error);
if(X_matrix==NULL) goto catch_error; // error matrix creation wale ne set kr di hogi

// fill the first col of input matrix
sss_matrix_fill(X_matrix,0,0,X_matrix_rows-1,0,1.0,NULL); //error ki jagah null pass kiya

// now copy the input from x to X for (0,1) to (n-1,n-1)
sss_matrix_copy(X_matrix,0,1, x_matrix,0,0,x_matrix_rows-1,x_matrix_columns-1,NULL);


// printf("print for testing for input matrix bnaya start\n");
// sss_matrix_print(stdout,X_matrix,error);
// printf("print for testing for input matrix bnaya ends\n");
// sss_on_error(error,catch_error);


// now create XT Y_HAT E ET ETE , here ete will calculat sum of squered error
// printf("print for original \n");
// sss_matrix_print(stdout,X_matrix,error);
// printf("print for original\n");
// sss_on_error(error,catch_error);
// printf("******************************************************************************************************************************\n");

XT_matrix=sss_matrix_transpose_create_new(X_matrix,error);
// printf("print for transpose create new \n");
// sss_matrix_print(stdout,XT_matrix,error);
// printf("print for  transpose create new\n");
// sss_on_error(error,catch_error);
// printf("******************************************************************************************************************************\n");
	

// sss_matrix_transpose(XT_matrix,error);
// printf("print for  transpose inplace \n");
// sss_matrix_print(stdout,XT_matrix,error);
// printf("print for transpose inplace\n");
// sss_on_error(error,catch_error);
// printf("******************************************************************************************************************************\n");


if(XT_matrix==NULL) goto catch_error;

Y_HAT_vector=sss_vector_create_new(y_vector_size,error);
if(Y_HAT_vector==NULL) goto catch_error;

E_vector=sss_vector_create_new(y_vector_size,error);
if(E_vector==NULL) goto catch_error;

ET_vector=sss_vector_create_new(y_vector_size,error);
if(ET_vector==NULL) goto catch_error;

ETE_vector=sss_vector_create_new(1,error);  // ye 1 size ka, iske liye ek function lgega , multiply vector get scalear
if(ETE_vector==NULL) goto catch_error;


//now humne decide kiya tha model vector size is equal to X matrix columns
model_vector_size = X_matrix_columns;
model_vector= sss_vector_create_new(model_vector_size,error);
if(model_vector==NULL) goto catch_error;

//now model me random fill kr dete  he
//sss_vector_fill_random(model_vector,error);


// modify vector to small value for unit testing instread of random
// sss_vector_set(model_vector,0,4991.511152,error);
// sss_vector_set(model_vector,1,0.28407,error);
// sss_vector_set(model_vector,2,0.071557,error);
// sss_vector_set(model_vector,3,-81.460425,error);
// sss_vector_set(model_vector,4,79.310792,error);
// sss_vector_set(model_vector,5,-8.129055,error);
// sss_vector_set(model_vector,6,-89.16411,error);
// sss_vector_set(model_vector,7,0.157015,error);

sss_vector_set(model_vector,0,4989.564632,error);
sss_vector_set(model_vector,1,0.389656,error);
sss_vector_set(model_vector,2,0.037823,error);
sss_vector_set(model_vector,3,-18.692858,error);
sss_vector_set(model_vector,4,-18.056414,error);
sss_vector_set(model_vector,5,-5.523223,error);
sss_vector_set(model_vector,6,-15.647461,error);
sss_vector_set(model_vector,7,0.501546,error);

// 4987.723666
// 0.390473
// 0.037331
// -18.344001
// -19.068995
// -5.498771
// -14.37405
// 0.508028












//ye upar wale ko hata k random k sath test krna he 


printf("print for testing for initial model bnaya start\n");
sss_vector_print(stdout,model_vector,error);
printf("print for testing for initial model bnaya ends\n");
// sss_on_error(error,catch_error);


// ab jo error vector aayega uska XT k sath multiply krege to m and c ki partial value aa jayegi
// and actual model vector me se is XTE ko minus kr dege to updated model aa jayega
XTE_vector=sss_vector_create_new(model_vector_size,error);   //
if(XTE_vector==NULL) goto catch_error;

// ab loop laga k XTE calculate kr k model update krna

for(i=1;i<=iterations;i++)
{
Y_HAT_vector=sss_multiply_matrix_with_vector(X_matrix,model_vector,Y_HAT_vector,error);
// printf("******************************************************************************************************************************\n");
// printf("print for testing for y hat  bnaya start  input and model ka multipy kr k\n");
// sss_vector_print(stdout,Y_HAT_vector,error);
// printf("print for testing for y hat  bnaya ends input and model ka multipy kr k \n");
// sss_on_error(error,catch_error);
// printf("******************************************************************************************************************************\n");

// E= Yhat_vector - y_vector
E_vector=sss_substract_vector(Y_HAT_vector,y_vector,E_vector,error);
// printf("print for testing for y hat  bnaya start E vector\n");
// sss_vector_print(stdout,E_vector,error);
// printf("print for testing for y hat  bnaya ends E vector \n");
// sss_on_error(error,catch_error);
// printf("******************************************************************************************************************************\n");

ET_vector =  sss_vector_transpose(E_vector,ET_vector,error);
// printf("print for testing for y hat  bnaya start  ET vector\n");
// sss_vector_print(stdout,ET_vector,error);
// printf("print for testing for y hat  bnaya ends  ET vector \n");
// sss_on_error(error,catch_error);
// printf("******************************************************************************************************************************\n");

error_value=sss_multiply_vector_get_scalar(ET_vector,E_vector,error);
if(i%100 == 0)  // %1000 %500 %500000 if data is 10000000 based on the data length
printf("Error : %10.5f\n",error_value);
// printf("******************************************************************************************************************************\n");


XTE_vector = sss_multiply_matrix_with_vector(XT_matrix,E_vector,XTE_vector,error);
// printf("print for testing for y hat  bnaya start  XTE_vector\n");  // come here 
// sss_vector_print(stdout,XTE_vector,error);
// printf("print for testing for y hat  bnaya ends  XTE_vector \n");
// sss_on_error(error,catch_error);
// printf("******************************************************************************************************************************\n");

XTE_vector = sss_multiply_vector_with_scalar(XTE_vector,((1.0)/(X_matrix_rows))*learning_rate,XTE_vector,error);
// printf("print for testing for y hat  bnaya start  XTE_vector after learning rate\n");
// sss_vector_print(stdout,XTE_vector,error);
// printf("print for testing for y hat  bnaya ends  XTE_vector after learning rate \n");
// sss_on_error(error,catch_error);
			
// printf("******************************************************************************************************************************\n");

model_vector =  sss_substract_vector(model_vector,XTE_vector,model_vector,error);
// printf("print for testing for y hat  bnaya start  model_vector after learning rate\n");
// sss_vector_print(stdout,model_vector,error);
// printf("print for testing for y hat  bnaya ends  model_vector after learning rate \n");
// sss_on_error(error,catch_error);
}



goto prepare_for_exit;
    catch_error:
		sss_get_error(error_message,100,error);
		printf("Error is : %s ",error_message);
		
        sss_vector_destroy(model_vector, NULL);
    prepare_for_exit:
        sss_matrix_destroy(X_matrix, NULL);
        sss_matrix_destroy(XT_matrix, NULL);
        sss_vector_destroy(Y_HAT_vector, NULL);
        sss_vector_destroy(E_vector, NULL);
        sss_vector_destroy(ET_vector, NULL);
        sss_vector_destroy(XTE_vector, NULL);
return model_vector;
}


int main(int c,  char *v[])
{
char error_message[4096];
sss_err *error;

sss_matrix *dataset=NULL;  //read from csv
uint32_t dataset_rows,dataset_columns;

sss_matrix *input_matrix=NULL; //break a part into input
uint32_t input_matrix_rows,input_matrix_columns;

sss_vector *model=NULL;  //actual model
sss_vector *y= NULL;    // break second part into output vector
sss_vector *y_hat=NULL; // we will calculate it

double learning_rate =  atof(v[3]);
//double mean_squared_error;   // the mean of squered error

//uint32_t model_size;  // it should equal to input_matrix_column
uint64_t number_of_iterations;
uint64_t i;


char * filename;
filename =  v[1];
number_of_iterations=atol(v[2]);

//init error
error=sss_error_create_new();
if(error==NULL)
{
	printf("Low memory\n");
	return 0;
}

//load dataset into a matrix 
dataset=sss_matrix_read_csv(filename,0,error);   // skip lines 0
sss_on_error_throw(error,err);  // this macro will check error obj and then decide to go to err if some error
sss_matrix_get_dimensions(dataset,&dataset_rows,&dataset_columns,error);
//validate dataset, it should not be empty and should have atleast 2 columns [1 for feature that is input , 1 for output]

// printf("print for testing for case 1 main start\n");
// sss_matrix_print(stdout,dataset,error);
// printf("print for testing for case 1 main ends\n");

//split input and insert at index 0 filled with 1.0
input_matrix=sss_matrix_create_new(dataset_rows,dataset_columns,error); // why its size equal to dataset even we are removing the y column, ans: because we are removing y column from last index but adding new columns with 1 and shifting rest to next
sss_on_error_throw(error,err);
sss_matrix_get_dimensions(input_matrix,&input_matrix_rows,&input_matrix_columns,error);



//now fill the input_matrix with 1.0 and then copy the dataset data into it.

////sss_matrix_fill(input_matrix,0,0,input_matrix_rows-1,0,1.0,error);
// pass matrix , startrowindex, startcol index, endrow index, endcolindex, data to be placed, erro);

////sss_matrix_copy(input_matrix,0,1,dataset,0,0,dataset_rows-1,dataset_columns-2,error);
// input_matrix destination jisme copy krna he,0th row and 1st col se daalna start krna he
//then source, and start row and col index are 0,0  means starting se and khtm kaha krna he vo he row ka last index but col ka columns-2 because last wala to vector me jayega.


//create output vector (y) from dataset (last index)
y=sss_matrix_get_vector(dataset,0,dataset_columns-1,dataset_rows-1,dataset_columns-1,error);

// printf("print for testing for case 2 main start\n");
// sss_vector_print(stdout,y,error);
// printf("print for testing for case 2 main ends\n");
// let se 3*4 ka matrix is dataset to start (0,3) se start (2,3) pe end as per the above calculation
// means khadi patti nikalni


// now we can delete the dataset
//sss_matrix_destroy(dataset,error);

//create model vector
// model_size=input_matrix_columns;
// model=sss_vector_create_new(model_size,error);
//sss_on_error(error,err);  //ye macro abhi likha he isme do args pass kiya 1 kisme check , 2 check fail hua to kaha jana he
//sss_vector_fill_random(model,error); // initial model , kuch bhi le skte he
// sss_vector_print(model,error);
//sss_vector_write_csv(model,"actual.csv",error);


// training starts(logic to fit line)
for(i=0;i<number_of_iterations;++i)
{
//y_hat=sss_multiply_matrix_with_vector(input_matrix,model_vector,error);  // vo equation 1.c + m.x for given m and c and 1 and x as feature
//mean_squared_error=sss_mean_squared_error(y,y_hat,error);  // summmantion of squares of (y-y_hat) 
//printf("MSE: %10.12lf\n",mean_squared_error);
//sss_vector_destroy(y_hat,error);
// ye loop ko modify kr diya he part 6 page 35
//update model


// puran logic kachre k dabbe me
// nelow is the vectorized solution

// Y_HAT_vector= X_matrix_model_vector


}
model=sss_linear_regression_fit(dataset,y, number_of_iterations, learning_rate,error);
sss_vector_write_csv(model,"model.csv",error); // ye likhna baak he abhi
	goto exit_program;

	return 0;
err:
	sss_get_error(error_message,4096,error);
	printf("Error : %s\n",error_message);
	return 0;
exit_program:
		// destroy wala kaam, jaha bhi memory alocaiton ka kaaam hua
		sss_matrix_destroy(dataset,error);
		sss_matrix_destroy(input_matrix,error);
		sss_vector_destroy(y,error);
		sss_vector_destroy(y_hat,error);
		sss_vector_destroy(model,error);
		sss_error_destroy(error);
}