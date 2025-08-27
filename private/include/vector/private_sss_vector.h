#ifndef __PRIVATE__SSS__VECTOR__H__
#define __PRIVATE__SSS__VECTOR__H__
#include<stdio.h>
#include<stdint.h>
#include<sss_error.h>

typedef struct __sss_vector sss_vector;  // this is required other wise we will get the error, iski definintion to mainn file me hi rahegi
typedef struct __sss_matrix sss_matrix;

sss_matrix * sss_vector_get_matrix(const sss_vector *vector, sss_err *error); // vector has no property as data , instead it contains a matrix and that matrix have data.

void sss_vector_set_matrix(sss_vector *vector,sss_matrix *matrix, sss_err *error); // iski bhi jaruarat lgegi

#endif