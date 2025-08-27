#include<sss.h>
#include<private_sss.h>
#include<stdlib.h>

sss_matrix * sss_multiply_matrix(sss_matrix *left_matrix, sss_matrix *right_matrix, sss_matrix *product_matrix_container,sss_err *error)
{
    uint32_t left_matrix_rows,left_matrix_columns;
    uint32_t right_matrix_rows,right_matrix_columns;
    uint32_t product_matrix_rows, product_matrix_columns;
    uint32_t product_matrix_container_rows, product_matrix_container_columns;

    uint32_t r,c,j;
    double *left_matrix_data;
    double *right_matrix_data;
    double *product_matrix_data;

    double value;
    sss_matrix *product_matrix;
    char error_message[4096];
    if(error) sss_clear_error(error);
    if(left_matrix==NULL)
    {
        sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
        return NULL;
    }
    if(right_matrix==NULL)
    {
        sss_set_error(error,"Null pointer argument(2)",SSS_NULL_POINTER);
        return NULL;
    }
    sss_matrix_get_dimensions(left_matrix,&left_matrix_rows,&left_matrix_columns,error);
    sss_matrix_get_dimensions(right_matrix,&right_matrix_rows,&right_matrix_columns,error);
    if(left_matrix_columns!=right_matrix_rows)
    {
        sprintf(error_message,"Cannot multiply matrix [%u,%u]X[%u,%u]\n",left_matrix_rows,left_matrix_columns,right_matrix_rows,right_matrix_columns);
        sss_set_error(error,error_message,SSS_INVALID_DIMENSIONS_FOR_MATRIX_MULTIPLICATION);
        return NULL;
    }

    //below line is to calculte the size of product matrix
    product_matrix_rows=left_matrix_rows;
    product_matrix_columns=right_matrix_columns;  


    if(product_matrix_container==NULL)
    {
        product_matrix=sss_matrix_create_new(product_matrix_rows,product_matrix_columns,error);
        if(product_matrix==NULL) return NULL;
        //if contaier is null and after that we tried to create a new memory that will also nul then return null, no need to set error, kyuki ye to sss_create_new_matrix() me set kr di gayi hogi
    }
    else{
        // means container he,
        // now check if contaier size is valid or not
        product_matrix_container_rows=sss_matrix_get_row_count(product_matrix_container,error);
        product_matrix_container_columns=sss_matrix_get_column_count(product_matrix_container,error);
        if(product_matrix_rows!=product_matrix_container_rows || product_matrix_columns!=product_matrix_container_columns)
        {
            sprintf(error_message,"Invalid container size (%u,%u)",product_matrix_container_rows,product_matrix_container_columns);
            sss_set_error(error,error_message,SSS_INVALID_MATRIX_CONTAINER_DIMENSIONS);
            return NULL;
        }
        //if all good then assign the address of contaier to product_matrix pointer and now it will point to container
        product_matrix=product_matrix_container;
    }
    // finally we have space to write the product that is pointed by product_matrix 
    //now get the data of left and right matrix to read data from there
    left_matrix_data=sss_matrix_get_data(left_matrix,error);
    right_matrix_data=sss_matrix_get_data(right_matrix,error);

    // now get the pointer to product matrix data as well to write result there
    product_matrix_data=sss_matrix_get_data(product_matrix,error);

    // logic to multiply and put the result into product matrix

    for(r=0;r<product_matrix_rows;++r)
    {
        for(c=0;c<product_matrix_columns;++c)
        {
            value=0;
            // logic to multiply horizontal and vertical row and compute sum
            for(j=0;j<left_matrix_columns;++j) //or (j=0;j<right_matrix_rows;++j)  
            {
            //  left matrix ki horizontal strip ki length[left_matrix_columns] 
            //  right matrix ki vertical strip ki length[right_matrix_rows] 
            //value= value+ (left_matrix_data[] *  right_matrix_data[]);
            value= value+ (left_matrix_data[r*left_matrix_columns+j] *  right_matrix_data[j*right_matrix_columns +c]);
            //access value= rowindex*numberofcoulms_for_that_matrix + columnindex
            // for left matrix= r*left_matrix_columns+j  [left matrix k liye j col index ka kaam krega, kyuki left ki horizontal patti le rahe he and usme row index to constant rahega] 
            //for right matrix= j*rightmatrix_columns+c  [right matrix k liye j row index ka kaam krega, kyuki right ki vertical patti le rahe he and usme col index to constan rahega]                
            }
            product_matrix_data[r*product_matrix_columns +c]=value;
            // iske to rowindex colindex r and c hi rahege
        }
    }
return product_matrix;
}

sss_vector * sss_multiply_matrix_with_vector(sss_matrix *matrix, sss_vector *vector,sss_vector * product_vector_container,sss_err *error)
{
   char error_message[4096];
   uint32_t matrix_rows;
   uint32_t matrix_columns;
   uint32_t vector_size;
   uint32_t product_vector_container_size;
   sss_matrix *product_matrix;  // product matrix me store krege and then ye vector k matrix ko assign kr dege
   sss_vector *product_vector;
   if(error) sss_clear_error(error);
   if(matrix==NULL)
   {
    sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
    return NULL;
   }
   if(vector==NULL)
   {
    sss_set_error(error,"Null pointer argument(2)",SSS_NULL_POINTER);
    return NULL;
   }
   // since humne vector vertical bnanya he to usi hisab se chlna hoga.
   // is hisab se matrix always be a left operand and vector should be right operand
   // if above line is true then matrix columns should equal to vector size then multiplication possible

   sss_matrix_get_dimensions(matrix,&matrix_rows,&matrix_columns,error);
   vector_size=sss_vector_get_size(vector,error);
   if(matrix_columns!=vector_size)
   {
    sprintf(error_message,"Cannot multiply matrix (%u,%u) with vector (%u)",matrix_rows,matrix_columns,vector_size,error);
    sss_set_error(error,error_message,SSS_INVALID_DIMENSIONS_FOR_MATRIX_VECTOR_MULTIPLICATION);
    return NULL;
   }

   // now check for container
   // we are taking container in these functions because these functions will be invoked  in loop and every time memory creation will need so to avoid that , we just created on time memory and used that as a container
   if(product_vector_container==NULL)
   {
    product_vector=sss_vector_create_new(1,error); // 1 size ka kyu bnaya? ans: kyuki humko vo vector ka structure bnana he and usme size atlease 1 paas krni hi pdegi, 0 pr vo structure nahi bnayega.
    // ha issue ye he ki  structure k andr jo matrix bna he 1X1 ka  usko release nahi kiya he pure method me, kyuki structure k andr jo matrix pointer he vo to product matrix ko point krega jo sss_multiply_matrix() return krega.
    // container wale case me to us container se matrix nikal ke usko matrix_multiply ko as a container pass kr diya jata he to vo to useful ho jata he by writing product_matrix=ss_vector_get_matrix(product_vector_container,error);
    // but idr cotainer null aata he to hum ye 1X1 nahi paas kr rhe he , hum bhi NULL hi pass kr rahe he. by writing product_matrix=NULL
    if(product_vector==NULL) return NULL;
    // jaha humko reslt store krna he vo parameter me bhi nahi aaya nad naya bhi nahi bna to simply return
    product_matrix=NULL;
   } else {
    product_vector_container_size=sss_vector_get_size(product_vector_container,error);
    if(product_vector_container_size != matrix_rows) //kyuki final product ki size to no. of rows in left matrix k equal hogi. mXn * nXo = mXo where m is left operand rows and o is right operand columns and 0 will always 1.
    {

        sprintf(error_message,"Invalid container Size (%u)",product_vector_container_size);
        sss_set_error(error,error_message,SSS_INVALID_VECTOR_CONTAINER_SIZE);
        return NULL;
    }
    product_matrix=sss_vector_get_matrix(product_vector_container,error);
    product_vector=product_vector_container;
     
   }

     
   //sir ka code
//    product_matrix=sss_multiply_matrix(matrix,sss_vector_get_matrix(vector,error),product_matrix,error);
//    if(product_matrix==NULL) return NULL;
//    sss_vector_set_matrix(product_vector,product_matrix,error);

//come come, apna code , vo issue aa raha tha garbage wala
//reason : jb hum container paas krte he tb product_vector_container  k andr ka matrix and product matrix dono same location ko point kr rahe h e 
// and vector_set matrix me vo purane wale ko container k matrix ko free kr k product matrix assign kr rhe he , but dono same locatoin ko point kr rahe he to
// vo product matrix jisko point kr raha he vo to free ho gaya.
   product_matrix=sss_multiply_matrix(matrix,sss_vector_get_matrix(vector,error),product_matrix,error);
   if(product_matrix==NULL) return NULL;
   sss_vector_set_matrix(product_vector,product_matrix,error);

  return product_vector;
}
sss_vector * sss_substract_vector(const sss_vector *left_vector,const sss_vector *right_vector, sss_vector *diffrence_vector_container,sss_err *error)
{
    char error_message[4096] ;
    uint32_t i;
    uint32_t left_vector_size, right_vector_size,result_vector_size;
    uint32_t diffrence_vector_container_size;

    sss_vector *result_vector;
    double *left_data,*right_data,*result_data;

    if(error) sss_clear_error(error);
    if(left_vector==NULL)
    {
        sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
        return NULL;
    }
    if(right_vector==NULL)
    {
        sss_set_error(error,"Null pointer argument(2)",SSS_NULL_POINTER);
        return NULL;
    }

    // now check the size
    left_vector_size=sss_vector_get_size(left_vector,error);
    right_vector_size=sss_vector_get_size(right_vector,error);
    
    if(left_vector_size!=right_vector_size)
    {
        sprintf(error_message,"Unable to subract vector of size %u from vector of size %u",right_vector_size,left_vector_size);
        sss_set_error(error,error_message,SSS_VECTOR_SUBSTRACT_SIZE_ERROR);
        return NULL;
    }
    result_vector_size=left_vector_size;  //or result_vector_size=right_vector_size

    // check for container and its size
    if(diffrence_vector_container==NULL)
    {
        result_vector=sss_vector_create_new(result_vector_size,error);
        if(result_vector==NULL) return NULL;
    } 
    else
    {
        diffrence_vector_container_size=sss_vector_get_size(diffrence_vector_container,error);
        if(diffrence_vector_container_size!=result_vector_size)
        {
            sprintf(error_message,"Invalid container size (%u)",diffrence_vector_container_size);
            sss_set_error(error,error_message,SSS_INVALID_VECTOR_CONTAINER_SIZE);
            return NULL;
        }
        result_vector=diffrence_vector_container;
    }

    left_data=sss_matrix_get_data(sss_vector_get_matrix(left_vector,error),error);
    right_data=sss_matrix_get_data(sss_vector_get_matrix(right_vector,error),error);
    result_data=sss_matrix_get_data(sss_vector_get_matrix(result_vector,error),error);  // storage to write the difference result

    for(i=0;i<result_vector_size;++i)
    {
        // here leftdata, right data , resultdata are 2d array
        // and formula to access 2d arrya is rowindex*totalcolumns +columnindex 
        // here rowindex=i totalcolmns=1 because it is a vector and colindex is 0 coz only one column in vector 
        // so [rowindex*totalcolumns +columnindex] = [i*1 +0] = [i]
        result_data[i]= left_data[i]-right_data[i];
    }
    return result_vector;
}


double sss_multiply_vector_get_scalar(const sss_vector *left_vector,const sss_vector *right_vector,sss_err *error)
{
   char error_message[4096] ;
   uint32_t i;  
   double product;
   double *left_data, *right_data;
   uint32_t left_vector_size, right_vector_size;
   if(error) sss_clear_error(error);
   if(!left_vector)
   {
    sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
    return 0.0;
   }
    if(!right_vector)
   {
    sss_set_error(error,"Null pointer argument(2)",SSS_NULL_POINTER);
    return 0.0;
   }
   left_vector_size=sss_vector_get_size(left_vector,error);
   right_vector_size=sss_vector_get_size(right_vector,error);
   if(left_vector_size!=right_vector_size)
   {
    sprintf(error_message,"Cannot multiply vector of size (%u) with vector of size (%u) and produce a scalar value",left_vector_size,right_vector_size);
            sss_set_error(error,error_message,SSS_INVALID_DIMENSIONS_FOR_VECTOR_MULTIPLICATION);
            return 0.0;
   }
   left_data=sss_matrix_get_data(sss_vector_get_matrix(left_vector,error),error);
   right_data=sss_matrix_get_data(sss_vector_get_matrix(right_vector,error),error);

   for(i=0,product=0.0;i<left_vector_size;++i) product=product+(left_data[i]*right_data[i]);
   return product;
}
sss_vector * sss_multiply_vector_with_scalar(const sss_vector *left_vector,double right_value, sss_vector *product_vector_container,sss_err *error)
{
    char error_message[4096] ;
    uint32_t i;
    double value;
    sss_vector *product_vector;
   
    uint32_t left_vector_size, product_vector_container_size;
    if(error) sss_clear_error(error);
    if(!left_vector)
    {
        sss_set_error(error,"Null pointer",SSS_NULL_POINTER);
        return NULL;
    }
   left_vector_size=sss_vector_get_size(left_vector,error);
   if(product_vector_container==NULL)
   {
    product_vector=sss_vector_create_new(left_vector_size,error);
    if(product_vector==NULL) return NULL;
   }
   else
   {
    product_vector_container_size=sss_vector_get_size(product_vector_container,error);
    if(left_vector_size!=product_vector_container_size)
    {
        sprintf(error_message,"Invalid container size (%u)",product_vector_container_size);
        sss_set_error(error,error_message,SSS_INVALID_VECTOR_CONTAINER_SIZE);
        return NULL;
    }
    product_vector=product_vector_container;
   }

   for(i=0;i<left_vector_size;++i)
   {
    value=sss_vector_get(left_vector,i,error);
    value=value*right_value;
    sss_vector_set(product_vector,i,value,error);
   }
   return product_vector;

}


// functions reuire for r2score
double sss_sum_vector(const sss_vector *vector, sss_err *error)
{
    double sum;
    uint32_t i;
    uint32_t vector_size;
    if(error) sss_clear_error(error);
    if(vector==NULL)
    {
        sss_set_error(error,"Null pointer",SSS_NULL_POINTER);
        return 0.0;
    }
    vector_size=sss_vector_get_size(vector,error);
    for(i=0,sum=0.0;i<vector_size;++i) sum+=sss_vector_get(vector,i,error);
    return sum;
}
double sss_mean_vector(const sss_vector *vector, sss_err *error)
{
    double sum;
    uint32_t vector_size;
    if(error) sss_clear_error(error);
    sum=sss_sum_vector(vector,error); // yaha pr null checkk nahi kiya kyuki sum wala kr raha he , vo set krega to hum check kr rahe he next line r
    if(sss_has_error(error)) return 0.0;
    vector_size=sss_vector_get_size(vector,error);
    return sum/(double)vector_size;
}
sss_vector * sss_substract_scalar_from_vector(const sss_vector *vector,double value, sss_vector *diffrence_vector_container,sss_err *error)
{
    char error_message[4096] ;
    uint32_t i;
    uint32_t vector_size;
    uint32_t diffrence_vector_container_size;
    double difference_value;
    sss_vector *difference_vector; 
    if(error) sss_clear_error(error);
    if(vector==NULL)
    {
        sss_set_error(error,"Null pointer",SSS_NULL_POINTER);
        return NULL;
    }
    vector_size=sss_vector_get_size(vector,error);
    if(diffrence_vector_container==NULL)
    {
        difference_vector=sss_vector_create_new(vector_size,error);
        if(difference_vector==NULL) return NULL;
    }
    else{
        diffrence_vector_container_size=sss_vector_get_size(diffrence_vector_container,error);

    if(vector_size != diffrence_vector_container_size)
    {
        sprintf(error_message,"Invalid container size (%u)",diffrence_vector_container_size);
        sss_set_error(error,error_message,SSS_INVALID_VECTOR_CONTAINER_SIZE);
        return NULL;
    }
    difference_vector=diffrence_vector_container;
    }
    for(i=0;i<vector_size;++i)
    {
    difference_value= sss_vector_get(vector,i,error)-value;
    sss_vector_set(difference_vector,i, difference_value,error);
    }
    return difference_vector;
}
