#include<sss.h>
#include<private_sss.h>
#include<stdio.h>
#include<math.h>   // ye inbuilt wali math ki lib he

double sss_mean_squared_error(sss_vector *left_vector,sss_vector *right_vector,sss_err *error)
{
    double mean_squared_error;
    double *data,*left_data,*right_data;
    uint32_t i;
    sss_matrix *left_matrix=NULL;
    sss_matrix *right_matrix=NULL;
    sss_matrix *product_matrix=NULL;
    sss_matrix *matrix=NULL;

    sss_vector *difference = NULL;

    char error_message[4096];
    uint32_t left_size,right_size;

    if(error) sss_clear_error(error);
    if(left_vector==NULL)
    {
        sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
        return 0.0;
    }
     if(right_vector==NULL)
    {
        sss_set_error(error,"Null pointer argument(2)",SSS_NULL_POINTER);
        return 0.0;
    }
    left_size=sss_vector_get_size(left_vector,error);
    right_size=sss_vector_get_size(right_vector,error);
    if(left_size!=right_size)
    {
        sprintf(error_message,"Unable to compute mean square error with vector of size %u and %u",left_size,right_size);
        sss_set_error(error,error_message,SSS_VECTOR_SIZE_MISMATCH);
        return 0.0;
    }
        // callin below without container later covert it to call passign container in place of NULL
        difference=sss_substract_vector(left_vector,right_vector,NULL,error);
        if(difference==NULL)
        {
            // apply loop and calculate value
            mean_squared_error = 0.0;
            left_matrix=sss_vector_get_matrix(left_vector,error);
            right_matrix=sss_vector_get_matrix(right_vector,error);
            left_data= sss_matrix_get_data(left_matrix,error);
            right_data=sss_matrix_get_data(right_matrix,error);
            for(i=0;i<left_size;++i)
            {
                mean_squared_error+=pow((left_data[i]-right_data[i]),2.0);
            }
                return mean_squared_error/left_size;

          }  //vectorize solution , using transpose
            right_matrix=sss_vector_get_matrix(difference,error);
            left_matrix=sss_matrix_transpose_create_new(right_matrix,error);

            if(left_matrix==NULL) goto fallback;  // means transpose nahi hua because of memory createion faild , to phir se manul conpute
            
            //finally multipy m and m transpose
            product_matrix=sss_multiply_matrix(left_matrix,right_matrix,NULL,error); // ye code baad me container bna k sath bhi krna he , container wala code hum likh chuke he
            if(product_matrix==NULL)
            {
                // becase it will as well include memory createion
                // when we paad container then this line will be useless
                goto fallback;
            }

            // humare pass 1X1 ka product matrix he ab usme sum aa gaya he (0,0) pr ,to vo nikal lete he
            mean_squared_error=sss_matrix_get(product_matrix,0,0,error)/left_size;
            sss_vector_destroy(difference,error);
            sss_matrix_destroy(product_matrix,error);
            sss_matrix_destroy(left_matrix,error);
            sss_matrix_destroy(right_matrix,error);
            return mean_squared_error;

        fallback:
            matrix=sss_vector_get_matrix(difference,error);
            data=sss_matrix_get_data(matrix,error);
            mean_squared_error=0.0;
            for(i=0;i<left_size;++i)
            {
                mean_squared_error+=pow(data[i],2.0);
            }
            sss_vector_destroy(difference,error);
            sss_matrix_destroy(product_matrix,error);
            sss_matrix_destroy(left_matrix,error);
            sss_matrix_destroy(right_matrix,error);
            return mean_squared_error/left_size;

        
        
    
}

double sss_r2_score(const sss_vector *y_vector, const sss_vector *y_hat_vector,sss_err *error)
{

    char error_message[4096] ;
    uint32_t y_vector_size,y_hat_vector_size;
    sss_vector *difference_vector, *difference_vector_T;  
    double n,d,mean;  //n for numerator of formula, d for denominator of formula
    if(error) sss_clear_error(error);
    if(y_vector==NULL)
    {
        sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
        return 0.0;
    }
    if(y_hat_vector==NULL)
    {
        sss_set_error(error,"Null pointer argument(2)",SSS_NULL_POINTER);
        return 0.0;
    }

    y_vector_size=sss_vector_get_size(y_vector,error);
    y_hat_vector_size=sss_vector_get_size(y_hat_vector,error);

    // check ki both vector contains same no of elements
    if(y_vector_size != y_hat_vector_size)
    {
        sprintf(error_message,"Unable to compute R2scroe with vector of size (%u) and (%u)",y_vector_size,y_hat_vector_size);
        sss_set_error(error,error_message,SSS_VECTOR_SIZE_MISMATCH);
        return 0.0;
    }
    difference_vector= sss_substract_vector(y_vector,y_hat_vector, NULL,error);
    if(sss_has_error(error)) return 0.0;

    difference_vector_T = sss_vector_transpose(difference_vector,NULL,error);  // yaha container nahi tha to null pass kiya tha
    if(sss_has_error(error)) return 0.0;


    n=sss_multiply_vector_get_scalar(difference_vector_T,difference_vector,error); // sum of squred , numerator aa gaya
    // ab denominatro k liye mean nikal k usko usi me se substract krna

    mean=sss_mean_vector(y_vector,error);
    difference_vector=sss_substract_scalar_from_vector(y_vector,mean,difference_vector,error);  /// ye vector k har element se mean ko minus kr diya
    // now is ka transpose kr k isi iske sath multiply kr dege to sum of squerred aa jayega and that is denominator
    difference_vector_T = sss_vector_transpose(difference_vector,difference_vector_T,error); // yaha container  pass kiya he to
    d=sss_multiply_vector_get_scalar(difference_vector_T,difference_vector,error); // sum of squred , denominator aa gaya
    sss_vector_destroy(difference_vector,error);
    sss_vector_destroy(difference_vector,error);

    return 1.0-(n/d);
}