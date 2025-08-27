#ifndef __SSS__MATH__H__
#define __SSS__MATH__H__
#include<stdio.h>
#include<stdint.h>
#include<sss_matrix.h>
#include<sss_vector.h>
#include<sss_error.h>

sss_matrix * sss_multiply_matrix(sss_matrix *left_matrix, sss_matrix *right_matrix, sss_matrix *product_matrix_container,sss_err *error);

sss_vector * sss_multiply_matrix_with_vector(sss_matrix *matrix, sss_vector *vector,sss_vector * product_vector_container,sss_err *error);

sss_vector * sss_substract_vector(const sss_vector *left_vector,const sss_vector *right_vector, sss_vector *diffrence_vector_container,sss_err *error);


double sss_multiply_vector_get_scalar(const sss_vector *left_vector,const sss_vector *right_vector,sss_err *error);
sss_vector * sss_multiply_vector_with_scalar(const sss_vector *left_vector,double right_value, sss_vector *product_vector_container,sss_err *error);

double sss_sum_vector(const sss_vector *vector, sss_err *error);
double sss_mean_vector(const sss_vector *vector, sss_err *error);
sss_vector * sss_substract_scalar_from_vector(const sss_vector *vector,double value, sss_vector *diffrence_vector_container,sss_err *error);
#endif