#include<private_sss.h>  // here we are using seterror to humkoisko inclde krna pdega for compile time
#include<sss.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>


typedef struct __sss_vector
{
	sss_matrix *matrix;
}sss_vector;

sss_vector * sss_vector_create_new(uint32_t size,sss_err *error)
{
sss_vector *vector;
if(error) sss_clear_error(error);
if(size==0)
{ 
	sss_set_error(error,"Invalid size(0)",SSS_INVALID_SIZE);  //add in enum file
	return NULL;
}
//if all above good means we can create vector
vector=(sss_vector *)malloc(sizeof(sss_vector)); // is se matrix k pointer ki size ki mem allocate hogi means vector k naam pr humare paas only pointer to matrix he usi pr pura game khela he
if(vector==NULL)
{
	sss_set_error(error,"Low memory",SSS_LOW_MEMORY);
	return NULL;
}
// if vector is created then forward the call to matrix creation,
//here we want to create the column vector , which have only one columns so we pass second arg of matrix_create_new as 1
vector->matrix=sss_matrix_create_new(size,1,error);
if(vector->matrix==NULL)
{
	free(vector);  // jo memory allocate ho gayi thi jisme pointer k liye jagah bni thi vo freee kri  IMP
	return NULL;
}

return vector;
}


void __sss_vector_destroy(sss_vector **ptr_2_vector_ptr, sss_err *error)
{
//here ** liya coz hume user ne jo pointer pass kiya uska address chahiye, 
//jo ki um macro ki help se niche pass krege.
//note: draw the diagram for same in notes
	sss_vector *ptr;
	if(error) sss_clear_error(error);
	ptr=*ptr_2_vector_ptr;
	//ab yaha se ptr me poointer to pointer to matrix aa gaya 
	if(ptr==NULL) // means we will get address of pointer that pointing to vector
	{
	sss_set_error(error,"NULL POINTER",SSS_NULL_POINTER);
	return;
	}
	sss_matrix_destroy(ptr->matrix,error);
	free(ptr);
	*ptr_2_vector_ptr=NULL;
}

#define sss_vector_destroy(j,e) __sss_vector_destroy(&j,e); //user pass pointer to vector, and internally we used address of that pointer

void sss_vector_set(sss_vector *vector,uint32_t index,double value, sss_err *error)
{

char error_message[70]; // we took this coz we are going to use sprintf to prepare message which combine string and variable.
if(error) sss_clear_error(error);
//sprint f ko error_message dege ki yaha likho, and then message + variable in string. sprintf(error_message, "mesag %u heh %u",var1,var2) and so on..


// basic checks: 1. vector null 2. out of bound 3. -ve : ye to aayega nahi cox -ve converted to +ve coz of uint
if(vector==NULL)
{
 sss_set_error(error,"NULL POINTER",SSS_NULL_POINTER);
 return;
}

if(index>=sss_matrix_get_row_count(vector->matrix,error))
{
	sprintf(error_message,"Invalid index (%u)",index);
	sss_set_error(error,error_message,SSS_INVALID_INDEX); // add in enum
	return;
}
sss_matrix_set(vector->matrix,index,0,value,error);
}

double sss_vector_get(const sss_vector *vector,uint32_t index,sss_err *error)
{
char error_message[70];
if(error) sss_clear_error(error);
if(vector==NULL)
{
 sss_set_error(error,"NULL POINTER",SSS_NULL_POINTER);
 return 0.0;
}

if(index>=sss_matrix_get_row_count(vector->matrix,error))
{
	sprintf(error_message,"Invalid index (%u)",index);
	sss_set_error(error,error_message,SSS_INVALID_INDEX);
	return 0.0;
}
return sss_matrix_get(vector->matrix,index,0,error);
}

 
uint32_t sss_vector_get_size(const sss_vector *vector, sss_err *error)
{
	// in case of matrix dimension since we need to return two things that's why we told user to share the mem address so we can write there.
	// and user can read from there
	if(error) sss_clear_error(error);
	if(vector==NULL)
	{
		sss_set_error(error,"NULL POINTER",SSS_NULL_POINTER);
		return 0;  //matrix k case me multiple the dimension to addresses mngwa k us pr write kr diye the.
	}
	return sss_matrix_get_row_count(vector->matrix,error);
}


void sss_vector_print(FILE *file, const sss_vector *vector,sss_err *error)
{
	if(error) sss_clear_error(error);
	if(vector==NULL)
	{
	sss_set_error(error,"Null Pointer",SSS_NULL_POINTER);
	return;
	}
	sss_matrix_print(file,vector->matrix,error);
}

void sss_vector_write(const sss_vector *vector,const char *filename,sss_err *error)
{  
	if(error) sss_clear_error(error);
	if(vector==NULL) 
	{
		sss_set_error(error,"Null Pointer Argument(1)",SSS_NULL_POINTER);
		return;
	}
	if(filename==NULL) 
	{
		sss_set_error(error,"Null Pointer Argument(2)",SSS_NULL_POINTER);
		return;
	}
	sss_matrix_write(vector->matrix,filename,error);
	
}
sss_vector * sss_vector_read(const char *filename, sss_err *error)
{
	uint16_t error_code;
	sss_vector *vector;
	sss_matrix *matrix;
	uint32_t rows,columns;
	if(error) sss_clear_error(error);
	if(filename==NULL)
	{
	sss_set_error(error,"Null Pointer",SSS_NULL_POINTER);
	return NULL;
	}
	matrix=sss_matrix_read(filename,error);
	if(matrix==NULL)
	{
		// now we need to convert the error of matrix into error of vector
		// because usme dimesion ka jikr he
		if(error)
		{
			// ye errors sss_matrix_create_new ki call se aa skte he.
			error_code=sss_get_error_code(error);
			if(error_code==SSS_INVALID_DIMENSIONS || error_code ==SSS_INVALID_ROW_COUNT	|| error_code== SSS_INVALID_COLUMN_COUNT)
			{
				sss_clear_error(error);
				sss_set_error(error,"Invalid file format",SSS_FILE_FORMAT_ERROR);
			}
		}
		return NULL;
	}
	sss_matrix_get_dimensions(matrix,&rows,&columns,error);
			if(rows>1 && columns>1)
			{
				sss_matrix_destroy(matrix,error);  //release memory that alreadey created
				sss_set_error(error,"File contains matrix",SSS_FILE_CONTAINS_MATRIX);
				return NULL;
			}
			vector=(sss_vector *)malloc(sizeof(sss_vector));
			if(vector==NULL)
			{
			sss_matrix_destroy(matrix,error);
			sss_set_error(error,"Low memory",SSS_LOW_MEMORY);
			return NULL;
			}
			if(columns==1)
			{
				vector->matrix=matrix;
			}
			else
			{
			sss_matrix_transpose(matrix,error);
			vector->matrix=matrix;
			}
	return vector;
			
}
void sss_vector_fill_random(sss_vector *vector,sss_err *error)
{
	uint32_t i;
	double value;
	uint32_t size;
	if(error) sss_clear_error(error);
	if(vector==NULL){
		sss_set_error(error,"Null pointer",SSS_NULL_POINTER);
		return;
	}
	srand(time(0));
	size=sss_vector_get_size(vector,error);
	for(i=0;i<size;++i)
	{
		value=(double)rand();
		sss_matrix_set(vector->matrix,i,0,value,error); // vector k andr wale matrix me dalna he data that's why
	}
}
sss_matrix * sss_vector_get_matrix(const sss_vector *vector, sss_err *error)
{
	if(error) sss_clear_error(error);
	if(vector==NULL) 
	{
		sss_set_error(error,"Null pointer",SSS_NULL_POINTER);
		return NULL;
	}
	return vector->matrix;
}
void sss_vector_set_matrix(sss_vector *vector,sss_matrix *matrix, sss_err *error)
{
	if(error) sss_clear_error(error);
	if(vector==NULL) 
	{
		sss_set_error(error,"Null pointer Argument(1)",SSS_NULL_POINTER);
		return;
	}
	if(matrix==NULL) 
	{
		sss_set_error(error,"Null pointer Argument(2)",SSS_NULL_POINTER);
		return;
	}
	
	if(matrix!=vector->matrix)  sss_matrix_destroy(vector->matrix,error); // dono[jisko set krna he and jo already he] same nahi he tb hi destroy krna he
	
	// now assign the new matrix

	vector->matrix= matrix;

}
void sss_vector_write_csv(const sss_vector *vector,const char *filename,sss_err *error)
{
	if(error) sss_clear_error(error);
	if(vector==NULL)
	{
		sss_set_error(error,"Null pointer argument (1)",SSS_NULL_POINTER);
		return;
	}
	if(filename==NULL)
	{
		sss_set_error(error,"Null pointer argument (2)",SSS_NULL_POINTER);
		return;
	}
	sss_matrix_write_csv(vector->matrix,filename,error);
}
sss_vector * sss_vector_read_csv(const char *filename, uint8_t skip_lines, sss_err *error)
{
	// later on rectify the function with proper eror handling.
	sss_vector *vector=sss_vector_create_new(1,error);
	sss_matrix *matrix=sss_matrix_read_csv(filename,skip_lines,error);
	printf("printting matrix after read\n");
	sss_matrix_print(stdout,matrix,error);
	// sss_matrix_destroy(vector->matrix,error); //clear old
	sss_vector_set_matrix(vector,matrix,error);
	return vector;
}


sss_vector * sss_vector_transpose(const sss_vector *vector, sss_vector *transposed_vector_container,sss_err *error)
{
	char error_message[4096];
	sss_vector *transposed_vector=NULL;
	uint32_t transposed_vector_container_size;
	uint32_t sz;
	if(error) sss_clear_error(error);
	if(!vector)
	{
		sss_set_error(error,"Null pointe",SSS_NULL_POINTER);
		return NULL;
	}
	if(vector==transposed_vector_container) return transposed_vector_container; // user container and vector dono same pass kr diya, to hum to replica bhej rahe he to usi ko return kr diya
	//upar wale scenario me kya kya issues aa skte he dekhma he 
	sz=sss_vector_get_size(vector,error);
	if(transposed_vector_container==NULL)
	{
		transposed_vector=sss_vector_create_new(sz,error);
		if(transposed_vector==NULL)
		{
			sss_set_error(error,"Low memory",SSS_LOW_MEMORY);
			return NULL;
		}
	}
	else
	{
		transposed_vector_container_size=sss_vector_get_size(transposed_vector_container,error);
		if(transposed_vector_container_size!=sz)
		{
			sprintf(error_message,"Invalid container size (%u)",transposed_vector_container_size);
			sss_set_error(error,error_message,SSS_INVALID_VECTOR_CONTAINER_SIZE);
			return NULL;
		}
		transposed_vector=transposed_vector_container; // if me naya create kiya the else me contaier ka add assign kr diya agar container as per the requirement
	}
//	now copy the content from vector->matrix to transposed_vector->matrix
memcpy(sss_matrix_get_data(transposed_vector->matrix,error),sss_matrix_get_data(vector->matrix,error),sizeof(double)*sz);
// destination ka address , source ka address and kitna uthan he vo size 
return transposed_vector;
}