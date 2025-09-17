#ifndef __SSS__ENCODERS__H__ // we added H__ extra
#define __SSS__ENCODERS__H__
#include<stdio.h>
#include<stdint.h>
#include<sss_vector.h> // this is needed in get_vector method in source file, isme issue nahi aayega cyclic dependency wala coz inculde gaurd laga he.
#include<sss_error.h> // iska path compile time pr dege -I se

void sss_encoder_one_hot_encoding(const char *input_filename,const char *output_filename, sss_vector *columns, uint8_t skip_lines,sss_err *error);
void sss_encoder_binary_encoding(const char *input_filename,const char *output_filename, sss_vector *columns, uint8_t skip_lines,sss_err *error);
#endif
