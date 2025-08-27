#ifndef __SSS__ML__MATH__H__
#define __SSS__ML__MATH__H__
#include<stdio.h>
#include<stdint.h>
#include<sss_matrix.h>
#include<sss_vector.h>
#include<sss_error.h>
#include<sss_math.h>

double sss_mean_squared_error(sss_vector *left_vector,sss_vector *right_vector,sss_err *errror);
double sss_r2_score(const sss_vector *y_vector, const sss_vector *y_hat_vector,sss_err *error);
#endif