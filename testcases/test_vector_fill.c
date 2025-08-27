#include<sss.h>
int main()
{
sss_vector *v;
v=sss_vector_create_new(10,NULL);
sss_vector_fill_random(v,NULL);
sss_vector_print(stdout,v,NULL);
return 0;
}