#ifndef __SSS__VECTOR__H__
#define __SSS__VECTOR__H__
#include<stdio.h>
#include<stdint.h>
#include<sss_error.h>

typedef struct __sss_vector sss_vector;

sss_vector *sss_vector_create_new(uint32_t size,sss_err *error);
void __sss_vector_destroy(sss_vector **vector,sss_err *error);

#define sss_vector_destroy(j,e) __sss_vector_destroy(&j,e);

void sss_vector_set(sss_vector *vector,uint32_t index, double value, sss_err *error);
double sss_vector_get(const sss_vector *vector,uint32_t index,sss_err *error);

uint32_t sss_vector_get_size(const sss_vector *vector, sss_err *error);

//write to a file from vector inside memory 
void sss_vector_write(const sss_vector *vector,const char* filename,sss_err *error);
// read from file and convert to a vector
sss_vector * sss_vector_read(const char* filname, sss_err *error);
sss_vector * sss_vector_read_csv(const char *filename, uint8_t skip_lines, sss_err *error);
void sss_vector_write_csv(const sss_vector *vector,const char *filename,sss_err *error);
void sss_vector_print(FILE *file,const sss_vector *vector,sss_err *error);
void sss_vector_fill_random(sss_vector *vector,sss_err *error);

sss_vector * sss_vector_transpose(const sss_vector *vector, sss_vector *transposed_vector_container,sss_err *error);

#endif