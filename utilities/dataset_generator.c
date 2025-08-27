#include<sss.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

int main(int cc,char *vv[])
{
    //command line arguments filename no_of_rows no_of_columns
    char *filename;
    uint32_t r,c;
    uint32_t rows,columns;
    sss_matrix *matrix;
    int n,d;
    char error_message[4096];
    double value;
    sss_err *error;
    
    if(cc!=4)
    {
        printf("Usage :  dataset_generator filename rows columns\n");
        return 0;
    }
    filename=vv[1];
    rows=atol(vv[2]);
    columns=atol(vv[3]);
    error=sss_error_create_new();
    if(error==NULL)
    {
        printf("Low memory, could not create %s\n", filename);
        return 0;
    }
    matrix=sss_matrix_create_new(rows,columns,error);
    if(matrix==NULL)
    {
        printf("Low memory, could not create %s\n", filename);
        return 0; 
    }
    srand(time(0));
    for(r=0;r<rows;++r)
    {
        for(c=0;c<columns;++c)
        {
            n=rand();
            d=rand();
            if(d==0) d=3;  //for safe side
            value=(double)(n/d);
            sss_matrix_set(matrix,r,c,value,error);
        }
    }
    sss_matrix_write_csv(matrix,filename,error);
    if(sss_has_error(error))
    {
        sss_get_error(error_message,4096,error);
        printf("Unable to create %s",error_message);

    }
    else
    {
        printf("Created %s",filename);
    }
    sss_matrix_destroy(matrix,error);
    sss_error_destroy(error);
    return 0;

    // isko compile krne k liye
    // gcc abc.c  -I  ../include –l ../lib* -lsss –lm –o abc.out  -Wall
    // isko testcases folder k root pr chalan he
    
}