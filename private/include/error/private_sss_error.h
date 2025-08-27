#ifndef __PRIVATE__SSS__ERROR__H__
#define __PRIVATE__SSS__ERROR__H__
#include<stdio.h>
#include<stdint.h>

typedef struct __sss_err sss_err;

void sss_set_error(sss_err *error, const char *error_str, uint16_t fallback_error_code);
#endif