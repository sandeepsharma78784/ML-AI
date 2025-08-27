#include<sss.h>
#include<stdio.h>

int main()
{
	char error_message[101];
	uint32_t r,c;
	double value;
	uint32_t rows,columns;
	sss_matrix *matrix1;
	sss_matrix *matrix2;

    sss_vector *vector1;
    sss_vector *vector2;


	sss_matrix *product_matrix_container;
    sss_vector *product_vector_container;
    sss_vector *diffrence_result_vector_container;

	sss_err *error;
    error=sss_error_create_new();
	if(error==NULL)
	{
		printf("Out of memory\n");
		return 0;
	}
    // negative testin : invalid dimension for multipliation
        // matrix1 = sss_matrix_create_new(2,2,error);
        // matrix2 = sss_matrix_create_new(1,1,error);
        // vector1 = sss_vector_create_new(2)   // multiply will be done with matrix1

	matrix1 = sss_matrix_create_new(2,2,error);
	matrix2 = sss_matrix_create_new(2,3,error);
    vector1 = sss_vector_create_new(2,error);          // multiply will be done with matrix1 whose col size is 2 that why we capt size of vector as 2(means 1 col 2 rows in matrix)         
    vector2 = sss_vector_create_new(2,error);           // for substract vector test it should be of same size , for negetive testting change the size


	sss_on_error_throw(error,err);

    //filll data into matrix 1
	sss_matrix_set(matrix1,0,0,10.0,error);
	sss_matrix_set(matrix1,0,1,15.0,error);
	sss_matrix_set(matrix1,1,0,2.0,error);
	sss_matrix_set(matrix1,1,1,3.0,error);

    //filll data into matrix 2
    sss_matrix_set(matrix2,0,0,10.0,error);
	sss_matrix_set(matrix2,0,1,15.0,error);
	sss_matrix_set(matrix2,0,2,1.0,error);
	sss_matrix_set(matrix2,1,0,2.0,error);
	sss_matrix_set(matrix2,1,1,3.0,error);
	sss_matrix_set(matrix2,1,2,5.0,error);

    //filll data into vector 1
    sss_vector_set(vector1,0,2.0,error);
    sss_vector_set(vector1,1,5.0,error);

    //filll data into vector 2
    sss_vector_set(vector2,0,3.0,error);
    sss_vector_set(vector2,1,4.0,error);


    sss_on_error_throw(error,err);

    //matrix test by passing container as NULL
        // product_matrix_container=sss_multiply_matrix(matrix1,matrix2,NULL,error);
        // sss_on_error_throw(error,err);

    //matrix test by passing container of invalid size : negetive testing
        //product_matrix_container= sss_matrix_create_new(2,1,error);

    //matrix test by passing container of valid size : positive testing
        product_matrix_container= sss_matrix_create_new(2,3,error);
        sss_multiply_matrix(matrix1,matrix2,product_matrix_container,error);
        sss_on_error_throw(error,err);


        //print product 
        sss_matrix_print(stdout,product_matrix_container,error);
        printf("matrix*matrix ends\n\n");



// multiply matrix with matrix testing ends.


// multiply matrix with vector testing start****************************************************************************

    //matrix_vectot multi test by passing container as NULL
        // product_vector_container=sss_multiply_matrix_with_vector(matrix1,vector1,NULL,error);
        // sss_on_error_throw(error,err);
   
    //matrix_vectot test by passing container of invalid size : negetive testing
        //product_vector_container= sss_vector_create_new(4,error);
    
    //matrix_vectot test by passing container of valid size : positive testing
     uint32_t s;
        product_vector_container= sss_vector_create_new(2,error);
        s=sss_vector_get_size(product_vector_container,error);
        // printf(" %u product_vector_container size before multiplytestcase\n",s);
        // product_vector_container=sss_multiply_matrix_with_vector(matrix1,vector1,product_vector_container,error);
        product_vector_container=sss_multiply_matrix_with_vector(matrix1,vector1,NULL,error);
        s=sss_vector_get_size(product_vector_container,error);
        // printf(" %u product_vector_container size after multiply testcase\n",s);
        

        sss_on_error_throw(error,err);

    //print product  
        printf("matrix*vector start\n");
        sss_vector_print(stdout,product_vector_container,error);
        sss_on_error_throw(error,err);
        printf("matrix*vector ends\n\n");

//note : do more negative testing

// multiply matrix with vector  testing ends*****************************************************************************




       
/* diffrence vector test start***************************************************************************    */

diffrence_result_vector_container= sss_vector_create_new(2,error);
sss_substract_vector(vector1, vector2,diffrence_result_vector_container,error);
sss_vector_print(stdout,diffrence_result_vector_container,error);

// printf("vector - vector start\n")
/* diffrence vector test ends**************************************************************************************    */



    // gcc test_math.c -I ../include -I ../include/error -I ../include/vector  -I ../include/matrix  -L ../lib -lsss -o test_math.out
    goto exit;
    err:
        sss_get_error(error_message,101,error);
        printf("some error:  %s",error_message);
    
    exit: 
        sss_matrix_destroy(matrix1,error);
        sss_matrix_destroy(matrix2,error);
        sss_matrix_destroy(product_matrix_container,error);

        sss_vector_destroy(vector1,error);
        sss_vector_destroy(vector2,error);
        sss_vector_destroy(product_vector_container,error);
        sss_vector_destroy(diffrence_result_vector_container,error);

        sss_error_destroy(error);

    return 0;




}