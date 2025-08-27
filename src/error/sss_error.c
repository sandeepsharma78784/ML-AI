#include<sss.h>
#include<stdlib.h>
#include<string.h>

typedef struct __sss_err
{
uint16_t fallback_error_code;
char *error_str;
}sss_err;

sss_err * sss_error_create_new()
{
	sss_err *error;
	error=(sss_err *)malloc(sizeof(sss_err));
	if(error==NULL) return NULL;
	error->error_str=NULL;
	error->fallback_error_code=0;
	return error;
}

void __sss_error_destroy(sss_err **error)
{
	if(error==NULL || *error==NULL) return;
	if((*error)->error_str) free((*error)->error_str);
	free(*error);
	*error=NULL;
}

uint8_t sss_has_error(const sss_err *error)
{
	if(error==NULL) return 0;
	if(error->error_str==NULL && error->fallback_error_code==0) return 0; // fallback error code in case ki error str generate nahi hua due to malloc fail 
	else return 1;
}

uint32_t sss_get_error_len(const sss_err *error)
{
	if(error==NULL) return 0; //no message
	if(error->error_str == NULL && error->fallback_error_code==0) return 0;
	if(error->error_str!=NULL) return strlen(error->error_str);
	return 5;  // means yaha aaye to  fallback error code ki baat aayi jo max 5 digit ka ho skta he, as per uint16_t 
		   // yaha pr 6 return kr skte the ek bit \0 k liye but vo user khud dega
		  // user krega sss_get_error(dest,sss_get_error_len(error)+1), error);
}

void sss_get_error(char *str,uint32_t size, const sss_err *error)
{
	uint32_t i;
	uint32_t endpoint;
	if(str && size>0) *str='\0'; //default;
	if(!error || !str || size==0) return; 
	if(error->error_str==NULL && error->fallback_error_code==0) return;
	if(error->error_str!=NULL)
	{
	endpoint=size-1;    //last index
	for(i=0;i<endpoint && error->error_str[i]!='\0'; ++i)
	{
		str[i]=error->error_str[i];
	}
	str[i]='\0';
	}
	else
	{
		if(size<6) return;   // 5 for fallback error code digits and 1 for \0
		sprintf(str,"%u", error->fallback_error_code); // yaha pr sprintf and me \0 bhi lga dega and see how it works
	}
	
}

void sss_clear_error(sss_err *error)
{
	if(!error) return;
	if(error->error_str)
	{
		free(error->error_str);
		error->error_str = NULL;
	}
	error->fallback_error_code=0; // default value for fallback error code
}

void sss_set_error(sss_err *error, const char *error_str, uint16_t fallback_error_code)
{
	if(!error) return;
	if(sss_has_error(error)) sss_clear_error(error); //pichla wala clear krna, isko to har line pr declaration k baad clear hi krna he 
	error->fallback_error_code = fallback_error_code;
	error->error_str=(char *)malloc(strlen(error_str)+1);
	if(error->error_str==NULL) return; // yaha se return means user ka message set nahi hoga but error code to set ho chuka hoga. isliye no issue
	strcpy(error->error_str,error_str); // source and destination k base addresses
} 

uint16_t sss_get_error_code(const sss_err *error)
{
	if(error==NULL) return 0;
	return error->fallback_error_code;
}



