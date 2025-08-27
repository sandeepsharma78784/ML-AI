#ifndef __SSS__ERROR__H__
#define __SSS__ERROR__H__
#include<stdio.h>
#include<stdint.h>

#define SSS_LOW_MEMORY 1
#define SSS_INVALID_DIMENSIONS 2
#define SSS_INVALID_ROW_COUNT 3
#define SSS_INVALID_COLUMN_COUNT 4
#define SSS_NULL_POINTER 5
#define SSS_INVALID_ROW_AND_COLUMN_INDEX 6
#define SSS_INVALID_ROW_INDEX 7
#define SSS_INVALID_COLUMN_INDEX 8
#define SSS_FILE_WRITE_ERROR 9
#define SSS_FILE_READ_ERROR 10
#define SSS_CSV_FORMAT_ERROR 11
#define SSS_CSV_EMPTY_ERROR 12

// new for vector
#define SSS_INVALID_SIZE 13
#define SSS_INVALID_INDEX 14
#define SSS_FILE_FORMAT_ERROR 15
#define SSS_FILE_CONTAINS_MATRIX 16
#define SSS_GET_VECTOR_ERROR 17
#define SSS_INVALID_DIMENSIONS_FOR_MATRIX_MULTIPLICATION 18
#define SSS_INVALID_DIMENSIONS_FOR_MATRIX_VECTOR_MULTIPLICATION 19
#define SSS_VECTOR_SUBSTRACT_SIZE_ERROR 20
#define SSS_VECTOR_SIZE_MISMATCH 21

#define SSS_INVALID_MATRIX_CONTAINER_DIMENSIONS 22
#define SSS_INVALID_VECTOR_CONTAINER_SIZE 23


#define SSS_INVALID_NUMBER_OF_ITERATIONS 24
#define SSS_INPUT_AND_TARGET_OUTPUT_SIZE_MISMATCH 25
#define SSS_INVALID_DIMENSIONS_FOR_VECTOR_MULTIPLICATION 26
#define SSS_INVALID_MODEL_SIZE 27

// #define SSS_INVALID_SIZE 17

// lets define macro to throw the error/exception
#define sss_on_error_throw(a,b) if(sss_has_error(a)) goto b  //end me semicolon nahi dena he
#define sss_on_error(a,b) if(sss_has_error(a)) goto b  //end me semicolon nahi dena he

typedef struct __sss_err sss_err; // note : if we put 3 underscore in __sss_err OR change the name of struct then it will give double declaration error for sss_err while we compile c file where we define the struct

sss_err * sss_error_create_new();              			   	// create, it will return address of created struct
void __sss_error_destroy(sss_err **error);	 		  	// it will take address of pointer tha point to error struct, it is for internal use
#define sss_error_destroy(error) __sss_error_destroy(&error)	 //macro, user will call sss_error_destroy() and pass a pointer to struct , now internaly we will pass address of that pointer coz we want to modify the value of pointer as well
uint8_t sss_has_error(const sss_err * error); 				// return one byte 8 bit[int value], error ko koi modify na krei iske liye const
void sss_get_error(char *str,uint32_t size, const sss_err *error);     // uint32_t because length of error message can be 
void sss_clear_error(sss_err *error);
uint32_t sss_get_error_len(const sss_err *error);
uint16_t sss_get_error_code(const sss_err *error);

#endif


