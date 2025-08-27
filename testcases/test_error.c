#include<private_sss.h>
#include<sss.h>
#include<stdio.h>

// below function is just for testing remove it while creating lib
int main()
{
sss_err *error;
error=sss_error_create_new();
char err_message[100];

sss_set_error(error,"one error there",3);
sss_get_error(err_message,100,error);
uint8_t haserror= sss_has_error(error);
uint16_t errorcode =  sss_get_error_code(error);
uint32_t errorlen = sss_get_error_len(error);
printf("%s %u %u %u\n", err_message, errorlen,errorcode,haserror);

sss_clear_error(error);

sss_get_error(err_message,100,error);
haserror= sss_has_error(error);
errorcode =  sss_get_error_code(error);
errorlen = sss_get_error_len(error);

printf("%s %u %u %u\n", err_message, errorlen,errorcode,haserror);


sss_error_destroy(error);
return 0;
}