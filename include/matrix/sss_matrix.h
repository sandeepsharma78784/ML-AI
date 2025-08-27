#ifndef __SSS__MATRIX__H__ // we added H__ extra
#define __SSS__MATRIX__H__
#include<stdio.h>
#include<stdint.h>
#include<sss_vector.h> // this is needed in get_vector method in source file, isme issue nahi aayega cyclic dependency wala coz inculde gaurd laga he.
#include<sss_error.h> // iska path compile time pr dege -I se

typedef struct __sss_matrix sss_matrix;

sss_matrix * sss_matrix_create_new(uint32_t rows, uint32_t columns,sss_err *error);  //create

void __sss_matrix_destroy(sss_matrix **matrix,sss_err *error);   //destroy
#define sss_matrix_destroy(j,e) __sss_matrix_destroy(&j,e);  // call forwarding by modifying the incoming parameter as per our need, semicolon is required here otherwise we will get erorr at compile time

void sss_matrix_set(sss_matrix *matrix,uint32_t row, uint32_t column, double value, sss_err *error);  //set value
double sss_matrix_get(const sss_matrix *matrix, uint32_t row, uint32_t column,sss_err *error);  	 // get value

void sss_matrix_get_dimensions(const sss_matrix *matrix,uint32_t *rows,uint32_t *columns,sss_err *error); // we are taking information inplace so return void
uint32_t sss_matrix_get_row_count(const sss_matrix *matrix, sss_err *error);
uint32_t sss_matrix_get_column_count(const sss_matrix *matrix, sss_err *error);

void sss_matrix_write(const sss_matrix *matrix,const char *filename,sss_err *error);
sss_matrix * sss_matrix_read(const char *filename, sss_err *error);
void sss_matrix_write_csv(const sss_matrix *matrix,const char *filename,sss_err *error);
sss_matrix * sss_matrix_read_csv(const char *filename,uint8_t skip_lines,sss_err *error);


void sss_matrix_transpose(sss_matrix *matrix,sss_err *error);
sss_matrix * sss_matrix_transpose_create_new(const sss_matrix *matrix,sss_err *error);

//new functions apart from basic functions
void sss_matrix_print(FILE *f,const sss_matrix *matrix, sss_err *error);

// now add the functions  fill(), copy() and get_vector()  functions
void sss_matrix_fill(sss_matrix *input_matrix,uint32_t start_row_index,uint32_t start_column_index,uint32_t end_row_index,uint32_t end_column_index,double fill_value,sss_err *error);
void sss_matrix_copy(sss_matrix *target,uint32_t target_start_row_index,uint32_t target_start_column_index,const sss_matrix *source,uint32_t source_start_row_index,uint32_t source_start_column_index,uint32_t source_end_row_index,uint32_t source_end_column_index,sss_err *error);
sss_vector * sss_matrix_get_vector(const sss_matrix *source,uint32_t source_start_row_index,uint32_t source_start_column_index,uint32_t source_end_row_index,uint32_t source_end_column_index,sss_err *error);
#endif
