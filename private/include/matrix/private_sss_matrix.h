#ifndef __PRIVATE__SSS__MATRIX__H__
#define __PRIVATE__SSS__MATRIX__H__
#include<stdio.h>
#include<stdint.h>
#include<sss_error.h>

typedef struct __sss_matrix sss_matrix;  // this is required other wise we will get the error, iski definintion to mainn file me hi rahegi

double * sss_matrix_get_data(const sss_matrix *matrix, sss_err *error);

#endif