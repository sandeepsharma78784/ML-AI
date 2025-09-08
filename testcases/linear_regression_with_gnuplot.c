#include<sss.h>
#include<stdio.h>
#include<stdlib.h>  // we used atol()
#include <windows.h>  // Sleep()
// since we are writng the code as consumer of library to we aare not allowe to use seterror coz it is a private function
// but humne use kiya he to usko idr include to krna pdega
#include<private_sss.h> // ye hata dege to seterror k liye compile time error aayga

// sss_vector * sss_linear_regression_fit(const sss_matrix *x_matrix,const sss_vector *y_vector,uint64_t iterations,double learning_rate, sss_err *error)
sss_vector * sss_linear_regression_fit(const sss_matrix *x_matrix, sss_vector *y_vector,uint64_t iterations,double learning_rate, FILE * gnuplot,FILE * ploterror,sss_err *error)
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

 double slope,intercept;
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
sss_matrix_fill(X_matrix,0,0,X_matrix_rows-1,0,1.0,NULL); //error ki jagah null pass kiya

// now copy the input from x to X for (0,1) to (n-1,n-1)
sss_matrix_copy(X_matrix,0,1, x_matrix,0,0,x_matrix_rows-1,x_matrix_columns-1,NULL);

XT_matrix=sss_matrix_transpose_create_new(X_matrix,error);

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

// take a random vlaues in model and then we will train the model we can take m=0 and c=0 as well
sss_vector_fill_random(model_vector,error); 

XTE_vector=sss_vector_create_new(model_vector_size,error);   //
if(XTE_vector==NULL) goto catch_error;
double slop[2000];
double cept[2000];
int k=0;

FILE *fppp = fopen("dataaaa.csv", "w");  // append to CSV
    if (!fppp) {
        printf("Error opening file!\n");
        return NULL;
    }
    fclose(fppp);



for(i=1;i<=iterations;i++)
{
Y_HAT_vector=sss_multiply_matrix_with_vector(X_matrix,model_vector,Y_HAT_vector,error);
E_vector=sss_substract_vector(Y_HAT_vector,y_vector,E_vector,error);
ET_vector =  sss_vector_transpose(E_vector,ET_vector,error);
error_value=sss_multiply_vector_get_scalar(ET_vector,E_vector,error);
if(i%5000 == 0)  // %1000 %500 %500000 if data is 10000000 based on the data length
{
printf("Error : %10.5f\n",error_value);
	
}
XTE_vector = sss_multiply_matrix_with_vector(XT_matrix,E_vector,XTE_vector,error);
XTE_vector = sss_multiply_vector_with_scalar(XTE_vector,((1.0)/(X_matrix_rows))*learning_rate,XTE_vector,error);
model_vector =  sss_substract_vector(model_vector,XTE_vector,model_vector,error);

if(i%5000==0)
{
     FILE *fpp = fopen("dataaaa.csv", "a");  // append to CSV
    if (!fpp) {
        printf("Error opening file!\n");
        return NULL ;
    }

        fprintf(fpp, "%lf, %lf\n", i/1.0,error_value);  // append x,y pairs

    fclose(fpp);



        slop[k++]= error_value ;
       cept[k++]= error_value;
       fprintf(ploterror,
    "plot 'dataaaa.csv' using 1:2 with points pt 7 ps 0.6 title 'Data'\n");
fflush(ploterror);
}

if(i%50000 == 0)
{
    slope=sss_vector_get(model_vector,0,NULL);
    intercept=sss_vector_get(model_vector,1,NULL);


   
// fprintf(ploterror, "plot $pointss using 1:2 with points pt 7 ps 1 title 'points'\n");
// fflush(ploterror);



	fprintf(gnuplot,
    //"plot 'line1000.csv' using 1:2 with points pt 7 ps 0.5 title 'Data', "
    "plot 'line1000.csv' using 1:2 with points pt 7 ps 1 title 'Data', "
    "%f*x+(%f) with lines lw 2 lc rgb 'red' title 'line'\n",
    slope, intercept);
       fflush(gnuplot);
 }
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
char command[256];
sss_matrix *dataset=NULL;  //read from csv
uint32_t dataset_rows,dataset_columns;

sss_matrix *input_matrix=NULL; //break a part into input

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
y=sss_matrix_get_vector(dataset,0,dataset_columns-1,dataset_rows-1,dataset_columns-1,error);
FILE *gnuplot, *ploterror;
sprintf(command, "\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persistent");
gnuplot=popen(command,"w");
ploterror=popen(command,"w");
if(gnuplot==NULL || ploterror==NULL)
{
perror("popen failed");
printf("issue\n");
return 0;
}



    // Setup gnuplot for line
    fprintf(gnuplot, "set datafile separator ','\n"); // CSV uses comma
    fprintf(gnuplot, "set title 'Data + Line'\n");
    fprintf(gnuplot, "set xlabel 'X'\n");
    fprintf(gnuplot, "set ylabel 'Y'\n");

    // Setup gnuplot for error
    fprintf(ploterror, "set datafile separator ','\n"); // CSV uses comma
    fprintf(ploterror, "set title 'Error Vs Iterations'\n");
    fprintf(ploterror, "set xlabel 'Iterations'\n");
    fprintf(ploterror, "set ylabel 'Error Value'\n");
    //  fprintf(ploterror, "set xrange [0:%d]\n",number_of_iterations);  
    // fprintf(ploterror, "set yrange [0:%d]\n",number_of_iterations);
    fflush(ploterror);



	// read data into a gnuplot variable databloakc
	// line1000.csv ko ek hi baar load karo points naam k datablock me
    // fprintf(gnuplot, "$points << EOD\n");
    // FILE *fp = fopen("line1000.csv", "r");
    // if (!fp) {
    //     perror("line1000.csv missing");
    //     return 1;
    // }
    // char buf[256];
    // while (fgets(buf, sizeof(buf), fp)) {
    //     fprintf(gnuplot, "%s", buf);
    // }
    // fclose(fp);
    // fprintf(gnuplot, "EOD\n");
    // fflush(gnuplot);


model=sss_linear_regression_fit(dataset,y, number_of_iterations, learning_rate,gnuplot,ploterror,error);
pclose(gnuplot);
pclose(ploterror);
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