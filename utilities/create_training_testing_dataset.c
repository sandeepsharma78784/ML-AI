#include<sss.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

int main(int cc,char *vv[])
{
    //command line arguments filename no_of_rows no_of_columns
    char *input_csv;
    char *training_csv;
    char *testing_csv;

    uint32_t i;
    char m;

    uint32_t upper_bound;
    uint32_t random_index;
    int skip_lines;
    int training_percentage;

    uint32_t input_csv_rows;
    FILE *inpuut_file,*training_file,*testing_file;

    uint32_t *array1,*array2;
    uint32_t g;
    uint32_t divider;


    if(cc!=6)
    {
        printf("Usage : create_training_testing_dataset.out input_dataset skip_lines training_dataset testing_dataset training_dataset_percentang");
        return 0;
    }

    input_csv=vv[1];
    skip_lines=atol(vv[2]);
    training_csv=vv[3];
    testing_csv=vv[4];
    training_percentage=atol(vv[5]);

    //count input rows , here input is the dataset jisko split krna he.
    inpuut_file=fopen(input_csv,"r");
    if(inpuut_file==NULL)
    {
        printf("Unable to read file %s\n",input_csv);
        return 0;
    }

    input_csv_rows=0;
    while(1){
        m=fgetc(inpuut_file);
        if(feof(inpuut_file)) break;
        if(m=='\n') input_csv_rows++;
        
    }
    input_csv_rows-=skip_lines;;
    rewind(inpuut_file);   // couted the row and placed the pointer again to the start
    array1=(uint32_t *)malloc(sizeof(uint32_t)*input_csv_rows);
    if(array1==NULL)
    {
        fclose(inpuut_file);
        printf("Low memory, unable to create training and testing datasets\n");
        return 0;   
    }

     array2=(uint32_t *)malloc(sizeof(uint32_t)*input_csv_rows);
    if(array2==NULL)
    {
        free(array1);
        fclose(inpuut_file);
        printf("Low memory, unable to create training and testing datasets\n");
        return 0;   
    }
    for(i=0;i<input_csv_rows;++i) array1[i]=i;  // using array i ndex we are spliting data
    srand(time(0));
    upper_bound=input_csv_rows-1; // end index
    while(upper_bound>10)  // idr 10 ki jagah kuch bhi le skte he 5,20,15 etc
    {
        random_index=rand()%upper_bound;
        g=array1[random_index];
        array1[random_index]=array1[upper_bound];
        array1[upper_bound]=g;
        upper_bound--;
    }
    for(i=0;i<input_csv_rows;++i) array2[array1[i]]=i; // understand this one 
    //now skip the lines
    while(skip_lines>0)
    {
        m=fgetc(inpuut_file);
        if(m=='\n') skip_lines--;
    }

    //now internal pointer is pointing to first byte of first data row
    training_file=fopen(training_csv,"w");
    if(training_file==NULL)
    {
        fclose(inpuut_file);
        free(array1);
        free(array2);
        printf("Unable to create %s\n",training_csv);
        return 0;
    }
    testing_file=fopen(testing_csv,"w");
    if(testing_file==NULL)
    {
        fclose(inpuut_file);
        fclose(training_file);
        free(array1);
        free(array2);
        printf("Unable to create %s\n",testing_csv);
        return 0;
    }


    // now do the split
    int index =0;
     divider =  input_csv_rows* training_percentage/100;
     divider--;
     printf("%u divider point %u total rows",divider,input_csv_rows);
    
    while(1)
    {
        m=fgetc(inpuut_file);
        if(feof(inpuut_file)) break;
        if(m=='\n')
        {
            if(array2[index]<=divider) fputc('\n',training_file);
            else  fputc('\n',testing_file);
            index++; // puri row read krne k baad hi index ++ kiya he
            continue;
        }
         if(array2[index]<=divider) fputc(m,training_file);
            else  fputc(m,testing_file);
    }
    fclose(inpuut_file);
    fclose(training_file);
    fclose(testing_file);
    printf("%s and %s created\n",training_csv,testing_csv);
    return 0;

}
