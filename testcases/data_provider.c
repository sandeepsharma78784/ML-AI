#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sss.h>
#include<private_sss.h>


// data provider function
typedef struct __sss_data_provider sss_data_provider;

typedef struct __sss_data_provider
{

    char *filename;
    FILE *file;
    uint32_t number_of_columns;
    uint8_t number_of_lines_in_header;
    void (*ptr_to_provider_function)(sss_data_provider *, sss_matrix *,uint32_t *rows_provided, sss_err *error);
    //(pointer name)(function signature (arguments of that function))
}sss_data_provider;


// ek esa fucntion likhte he jo data provider function bna k uska address de de taki , target user usko use kr ske
sss_data_provider * sss_data_provider_create(const char *filename,uint8_t number_of_lines_in_header,void (*ptr_to_provider_function)(sss_data_provider *,sss_matrix *,uint32_t *rows_provided,sss_err *),sss_err *error)
{
    // is function k paas provider se related saari info aayegi , ye vo saari infor us struct me wrap kr k return krega
    // 1. number of lines in header , of csv
    // 2. pointer to provider function
    // 3. filename , name of csv
    // 4. pointer to file after open
    // 5. number_of_columns  in csv
    // ye jo return krega uske andr function plus properties rahegi on which the function will work
    sss_data_provider *provider =NULL;
    if(error) sss_clear_error(error);
    if(filename==NULL)
    {
        sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
        return NULL;
    }
    if(ptr_to_provider_function==NULL)
    {
        // isko provider function ka address milega and uska use kr k ye data provide krega file se
        sss_set_error(error,"Null pointer argument(3)",SSS_NULL_POINTER);
        return NULL;
    }
    // open file check existence , if not available then return NULL
    provider=(sss_data_provider *)malloc(sizeof(sss_data_provider)); // means struct ka refrence and is me saaraiinfo rahegi 
    //printf("yaha tk");
    // lets wrap the infor like function and its properties.
    // ye bilkul class k object jesa hi bna diya humne c lang me 
    provider->number_of_lines_in_header=number_of_lines_in_header;
    provider->ptr_to_provider_function= ptr_to_provider_function;
    provider->filename=(char *)malloc(sizeof(char)*(strlen(filename)+1)); // copy kra filename 
    if(provider->filename==NULL)
    {
        // means filename rkhne ki jagah nahi bni 
        free(provider); // provider k saare variable free ho jayege, means struct
        sss_set_error(error,"Low memory",SSS_LOW_MEMORY);
        return NULL;
    }
   
    // ab vo file name copy kr do
    strcpy(provider->filename,filename);
    provider->file=NULL;
    provider->number_of_columns=0;

    return provider;    
}

// abhi upar structure ko create krne ka function 
// ab usi struct ko delete krne ka function

void sss_data_provider_destroy(sss_data_provider *provider, sss_err *error)
{
    // iske paas vo struct k address , iska kaam us me jitna bhi dynamic mem allocation a
    // he usko free krna and file ko close krna if open
    if(error) sss_clear_error(error);
    if(provider==NULL) 
    {
        sss_set_error(error,"Null pointer argument",SSS_NULL_POINTER);
        return;
    }
    if(provider->filename) free(provider->filename);
    if(provider->file) fclose(provider->file);
    // upar humne pointer jisko point kr rahe the usko free kiya , dynamic variables
    free(provider);  // to clear the static variables, like pointer,and other variable
}


// abhi scenario esa he ki hum ek space bnayege like matrix ,
// ab iska size kon decide kre? 
// jo isme data fill krwana chahta he data provider se
// to number of rows to ye matrix bnane wala decide kr skta he, 
// but number of columns kese pta krega?
// uske liye isko file open krni pdegi,
// ya phir ye kaam bhi data provider se hi krvaye
// to ye kaam data provider se hi krwate he

uint32_t sss_data_provider_get_column_count(sss_data_provider *provider,sss_err *error)
{
    // iske pass vo struct ka address aayega, 
    // to ye file open kr ke column count kr k struct ki number_of_columns property me bhi result update kr dega
    // and iske caller ko bhi result return kr dega

    int skip_lines;
    char error_message[4096];
    char m;
    uint32_t number_of_columns;
    if(error) sss_clear_error(error);
    // if struct itself null
    if(provider==NULL)
    {
        sss_set_error(error,"Null pointer argument",SSS_NULL_POINTER);
        return 0;
    }
    // printf("provider not null\n");
    if(provider->number_of_columns==0)
    {
        // first time open hogi file
        provider->file=fopen(provider->filename,"r");
        // printf("File opened\n");
        if(provider->file==NULL)
        {
            sprintf(error_message,"Unable to read file %s",provider->filename);
            sss_set_error(error,error_message,SSS_FILE_READ_ERROR);
            return 0;
        }
        skip_lines=provider->number_of_lines_in_header;
        while(skip_lines>0)
        {
            m=fgetc(provider->file); // ek character read kiya and fp pointer ko aage bdaya
            if(feof(provider->file)) break;
            if(m=='\n') skip_lines--;
        }

        // yadi abhi bhi skip  line me kuch bcha measn actual data kuch nahi he only header hi he sb
        if(skip_lines>0)
        {
            sprintf(error_message,"Invalid csv format of file %s",provider->filename);
            sss_set_error(error,error_message,SSS_CSV_FORMAT_ERROR);
            fclose(provider->file);
            provider->file=NULL;
            return 0;
        }
        // baat yaha tk pahuchi means data rows he,and we can provide then columns 
        number_of_columns=0;
        // abhi file pointer first data row k first index pr pda hoga lines skip krne k baaad
        while(1)
        {
            m=fgetc(provider->file);
            if(m=='\n' || feof(provider->file)) break;
            if(m==',') number_of_columns++; 
        }
        if(m!='\n')
        {
            sprintf(error_message,"Invalid csv format of file %s",provider->filename);
            sss_set_error(error,error_message,SSS_CSV_FORMAT_ERROR);
            fclose(provider->file);
            provider->file=NULL;
            return 0;
        }
        provider->number_of_columns=number_of_columns+1;
        fclose(provider->file);
        provider->file=NULL;
    }
    // if me nahi gaya control means number of columns phle hi count kr liye he
    return provider->number_of_columns;
}

void do_something_like_fit(sss_data_provider *provider,sss_err *error)
{
    // iske pass provider ka structure aayega
    uint32_t j,c;
    double value;
    uint32_t rows_filled_by_provider;
    sss_matrix *matrix=NULL;
    uint32_t matrix_rows;
    uint32_t number_of_columns;

    if(error) sss_clear_error(error);
    number_of_columns=sss_data_provider_get_column_count(provider,error);
    matrix_rows=500;   // we want to read 500 rows in one shot
    matrix=sss_matrix_create_new(matrix_rows,number_of_columns,error);
    if(sss_has_error(error))
    {
        printf("error while createint matrix %u, %u\n",matrix_rows,number_of_columns);
    }
    while(1)
    {
        provider->ptr_to_provider_function(provider,matrix,&rows_filled_by_provider,error);
        // do error check after each function call
        if(sss_has_error(error)) break;

        printf("Rows fetched %u\n",rows_filled_by_provider);
        for(j=0;j<rows_filled_by_provider;++j)
        {
            for(c=0;c<number_of_columns;++c)
            {
                value=sss_matrix_get(matrix,j,c,error);
            }
        }
        if(rows_filled_by_provider<matrix_rows) break; // means files ki saari rows read ho gayi coz last batch me ya to matrix ki size ki rows aayegi ya us se km
    }
sss_matrix_destroy(matrix,error);
}


// abhi vo provider function ki coding
void kalu(sss_data_provider *wrapper,sss_matrix *matrix,uint32_t *rows_provided,sss_err *error)
{
    char m;
    char str[51];
    double value;
    uint32_t r,c;
    int i;
    int number_of_columns;
    int skip_lines;
    char error_message[4096];
    uint32_t matrix_rows, matrix_columns;
    

    if(error) sss_clear_error(error);
    if(rows_provided) *rows_provided=0;  // every time need to set to 0 until we read the chunk 
    // once chunk read successful then count the provided rows and assign, [may be lesss than or equal to matrix rows]
    if(matrix==NULL)
    {
        // means container hi nulll he
        sss_set_error(error,"Null pointer argument",SSS_NULL_POINTER);
        return;
    }
    // printf("yaha tk ch raha he ");
    // get number of columns
    number_of_columns=sss_data_provider_get_column_count(wrapper,error);
     

    if(number_of_columns==0) 
    {
        // meand no data row found
        return;
    }

    // get dimensions of contaienr
    sss_matrix_get_dimensions(matrix,&matrix_rows,&matrix_columns,error);

    // size of contaier in terms of columns must match with columns in csv file
    // other wise partial data read hoga and in such scenario we will not filll data
    if(matrix_columns!=number_of_columns)
    {
            sprintf(error_message,"Invalid matrix size (%u,%u) required (%u, %u)",matrix_rows, matrix_columns, matrix_rows, number_of_columns);
            sss_set_error(error,error_message,SSS_INVALID_MATRIX_CONTAINER_DIMENSIONS);
            fclose(wrapper->file);
            wrapper->file=NULL;
            return;
    }
    // if contaier size if valid then chek if file is open or not
    // if not open then open the file
    if(wrapper->file==NULL)
    {
        wrapper->file=fopen(wrapper->filename,"r");
        if(wrapper->file==NULL)
        {
           sprintf(error_message,"Unable to read file %s",wrapper->filename);
            sss_set_error(error,error_message,SSS_FILE_READ_ERROR);
            return; 
        }
        skip_lines=wrapper->number_of_lines_in_header;
        // sinse we are opening file first time so header bhi skip krna pdega
        while(skip_lines>0)
        {
            m=fgetc(wrapper->file);
            if(feof(wrapper->file)) break;
            if(m=='\n') skip_lines--;
        }
    }


    // yaha pahuche mtlb file open he 
    // file pointer right row pr he
    // read file and filll matrix;
    i=0;
    r=0;
    c=0;
    while(r<matrix_rows)
    {
        m=fgetc(wrapper->file);
        if(feof(wrapper->file)) break;
        if(m==',' || m=='\n')
        {
            str[i]='\0';  // meand vo number ki ko as a str read kr rahe he
            i=0;
            value=atof(str);
            sss_matrix_set(matrix,r,c,value,error);
            c++;
            if(c==matrix_columns)
            {
                r++;
                c=0;                
            }
        } else {
            str[i++]=m; // har char ko str me dalte jao except ',' and '\n'
        }

    }

    // matrix fill kr diya file se
    if(rows_provided) *rows_provided=r;  // r ki jo value hogi utni rows file se utha k matrix me dali

    // ab ye file open hi rahegi , isko close kon krega?
    // jo bhi provider bnayega uski zimmedari hogi ki kaam khtm hone k badd usko destroy bhi kare
    // provider_destroy() me file close krne ka code likha he
    // e.g abhi niche main() me hum provider bnayege bhi and usko destroy bhi krege
    // provider is a struct, and provider function is a part of that struct
    // abhi humare case me kalu() is provider function
    // kalu end user ne bnaya he , framework wale ne nahi.
} 

int main() {
    sss_err *error;
    sss_data_provider *provider;
    error=sss_error_create_new();
    provider= sss_data_provider_create("trn.csv",0,kalu,error);
    // training file de di ,abhi skip lines 0 coz we dont have header
    if(sss_has_error(error))
    {
        printf("Some issue\n");  // error se actual error nikal k print krna
        return 0;
    }
    do_something_like_fit(provider,error);
    if(sss_has_error(error))
    {
        printf("Some issue\n");  // error se actual error nikal k print krna
        return 0;
    }
    // kaam hone k badd desroy provider
    sss_data_provider_destroy(provider,error);
    sss_error_destroy(error);
    return 0;
}