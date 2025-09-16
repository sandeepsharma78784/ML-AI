#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sss.h>
#include<private_sss.h>

#include<inttypes.h>


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


sss_data_provider * sss_data_provider_create(const char *filename,uint8_t number_of_lines_in_header,void (*ptr_to_provider_function)(sss_data_provider *,sss_matrix *,uint32_t *rows_provided,sss_err *),sss_err *error)
{
    sss_data_provider *provider =NULL;
    if(error) sss_clear_error(error);
    if(filename==NULL)
    {
        sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
        return NULL;
    }
    if(ptr_to_provider_function==NULL)
    {
        sss_set_error(error,"Null pointer argument(3)",SSS_NULL_POINTER);
        return NULL;
    }
    provider=(sss_data_provider *)malloc(sizeof(sss_data_provider)); // means struct ka refrence and is me saaraiinfo rahegi 
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

uint32_t sss_data_provider_get_column_count(sss_data_provider *provider,sss_err *error)
{
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
    return provider->number_of_columns;
}

void do_something_like_fit(sss_data_provider *provider,uint64_t iterations,double learning_rate, FILE * plotline,FILE * ploterror,sss_err *error)
{
    // iske pass provider ka structure aayega
    uint32_t j,c;
    double value;
    uint32_t rows_filled_by_provider;
    sss_matrix *matrix=NULL;
    uint32_t matrix_rows_to_be_fetched;   // this is sample size,it will be size of input matrix , 
    uint32_t number_of_columns;



    // declaration of variables for calculating MSE starts
        sss_matrix *X_matrix=NULL;
        sss_matrix *XT_matrix=NULL;

        sss_vector *Y_HAT_vector=NULL;
        sss_vector *E_vector=NULL;
        sss_vector *ET_vector=NULL;
        sss_vector *ETE_vector=NULL;
        sss_vector *XTE_vector=NULL;

        sss_vector *model_vector=NULL;
        sss_vector *y_vector=NULL;

        double error_value;
        uint64_t i;
        char error_message[4096];
        uint32_t x_matrix_rows,x_matrix_columns;
        uint32_t X_matrix_rows,X_matrix_columns;
        uint32_t y_vector_size;
        uint32_t model_vector_size;
    // declaration of variables for calculating MSE ends


    if(error) sss_clear_error(error);
    number_of_columns=sss_data_provider_get_column_count(provider,error);
    matrix_rows_to_be_fetched=10;   // we want to read 500 rows in one shot
    matrix=sss_matrix_create_new(matrix_rows_to_be_fetched,number_of_columns,error);
    if(sss_has_error(error))
    {
        printf("error while createing matrix %u, %u\n",matrix_rows_to_be_fetched,number_of_columns);
    }
// SGD
    if(iterations==0)
    {
        sss_set_error(error,"Cannot generate model with 0 iterations",SSS_INVALID_NUMBER_OF_ITERATIONS);
        goto catch_error;
    }

    // now we have number of columns= number_of_columns and  number of rows =matrix_rows_to_be_fetched
    // matrix in this function  is our dataset 
    // lets create infra
    
    
    // y vector matrix aane k baad bnega. container bna skte he, but container he nahi to har baar naya aayega and 
    // isko har baar while loop k end pr destroy krna pdega
    //matrix bhi har baar naya aayega ptr_to_provider_function se  ,uske liye container he
        // but size ka dhyan rkhna pdega for last chunk 
    // upar wale matrix se y vector niklega

    // ab X matrix  bnega upar wale matrix se , 1 in first colu , iska container nahi he te bhi delete hoga har baar
    // XT matrix, Yhat vector, Evecor, ET vector, ETE vector, XTE vector

    //  XT mat,,,, Yhat,E,ET,ETE,XTE are container
    // matrix,y_vector dynamic , destroy on end of while loop each time
    
    // since we have dataset matrix rows and columns , required to create all the containers
    // for mini batch we need to take care of last batch , for SGD rows always 1

    // we will get the row size in prameter instead of defineing here , later on we we parameterize it.

    x_matrix_rows= matrix_rows_to_be_fetched;
    x_matrix_columns= number_of_columns;

    X_matrix_rows= matrix_rows_to_be_fetched;
    X_matrix_columns= number_of_columns;

    y_vector_size= matrix_rows_to_be_fetched;


    // write belwo linw after   provider->ptr_to_provider_function(provider,matrix,&rows_filled_by_provider,error);
    //y_vector= sss_matrix_get_vector(matrix,0,x_matrix_columns-1,x_matrix_rows-1,x_matrix_columns-1,error);
    X_matrix=sss_matrix_create_new(X_matrix_rows,X_matrix_columns,error);
    if(X_matrix==NULL) goto catch_error; // error matrix creation wale ne set kr di hogi
    sss_matrix_fill(X_matrix,0,0,X_matrix_rows-1,0,1.0,NULL); //error ki jagah null pass kiya

    // X matrix bhi ek hi bar bnao , 2nd columns se data copy kr dege har baar, to ye niche wali line aayegi while loop me and
    // and ek y vector wali
   // sss_matrix_copy(X_matrix,0,1, matrix,0,0,x_matrix_rows-1,x_matrix_columns-1,NULL);

    // baki k contaiers


    //  container creation starts
            XT_matrix=sss_matrix_transpose_create_new(X_matrix,error);
            if(XT_matrix==NULL) goto catch_error;

            Y_HAT_vector=sss_vector_create_new(y_vector_size,error);
            if(Y_HAT_vector==NULL) goto catch_error;

            E_vector=sss_vector_create_new(y_vector_size,error);
            if(E_vector==NULL) goto catch_error;

            ET_vector=sss_vector_create_new(y_vector_size,error);
            if(ET_vector==NULL) goto catch_error;

            ETE_vector=sss_vector_create_new(1,error);  // ye 1 size ka, iske liye ek function lgega , multiply vector get scalear
            if(ETE_vector==NULL) goto catch_error;

            model_vector_size = X_matrix_columns;
            model_vector= sss_vector_create_new(model_vector_size,error);
            if(model_vector==NULL) goto catch_error;

            XTE_vector=sss_vector_create_new(model_vector_size,error);   //
            if(XTE_vector==NULL) goto catch_error;

            sss_vector_fill_random(model_vector,error); 
    // container creation ends


    // ab humare paas y matrix and data set he , abhi container bnane ka kaam

 //SGS

// for loop outside the while loop starts
long start_from= ftell(provider->file); // humne socha tha count me file open kr di hogi but vo to bnd kr di jayti he count k bad
    for(i=1;i<=iterations;i++)
    {
        if(i==0) break; // if limit cross of unit64
       // if(start_from!=-1)
        //fseek(provider->file,start_from,0);   //0 start of file, means 0 se start kro and data_start_at tk le jao f ko.
       
        fseek(provider->file,0,0); // start from begining for each loop. it willl fail in case of skip lines
        while(1)
        {
            provider->ptr_to_provider_function(provider,matrix,&rows_filled_by_provider,error);
            if(rows_filled_by_provider<matrix_rows_to_be_fetched) break; // we are skippping last batch if not equal to container size
            // if(start_from==-1)
            // start_from= ftell(provider->file); 


            // do error check after each function call
            // yaha pr vo algo chlegi  , sarre declarations dosomething linke fit me hi hoge
            
              
            //  create X XT matrix and y vector , niche wali method me container wali baat nahi he so ise destroy krna pdega
                y_vector= sss_matrix_get_vector(matrix,0,x_matrix_columns-1,x_matrix_rows-1,x_matrix_columns-1,error);
                sss_matrix_copy(X_matrix,0,1, matrix,0,0,x_matrix_rows-1,x_matrix_columns-1,NULL);
                 XT_matrix=sss_matrix_transpose_create_new(X_matrix,error);
            //  create X XT matrix and y vector
               
        
            // do the calculation and update model

            // print kr k dekh liya sb chl raha he 
            // printf("i=  %" PRIu64 "**************************************** dataset\n", i);
            // sss_matrix_print(stdout,matrix,error);
            // printf("############## X \n");
            // sss_matrix_print(stdout,X_matrix,error);
            // printf("############## y \n");
            // sss_vector_print(stdout,y_vector,error);
            // printf("##############----------------------------------------------------------------------------------------- model \n");
            // sss_vector_print(stdout,model_vector,error);

            // achi actual calculation start
                Y_HAT_vector=sss_multiply_matrix_with_vector(X_matrix,model_vector,Y_HAT_vector,error);
                if(sss_has_error(error)) 
                {
                 sss_get_error(error_message,4096,error);
                 printf(" here  %s\n", error_message);
                }
                E_vector=sss_substract_vector(Y_HAT_vector,y_vector,E_vector,error);
                ET_vector =  sss_vector_transpose(E_vector,ET_vector,error);
                error_value=sss_multiply_vector_get_scalar(ET_vector,E_vector,error);
                XTE_vector = sss_multiply_matrix_with_vector(XT_matrix,E_vector,XTE_vector,error);
                XTE_vector = sss_multiply_vector_with_scalar(XTE_vector,((1.0)/(X_matrix_rows))*learning_rate,XTE_vector,error);
                model_vector =  sss_substract_vector(model_vector,XTE_vector,model_vector,error);
            sss_vector_destroy(y_vector, NULL);
            sss_matrix_destroy(XT_matrix,NULL);

            if(sss_has_error(error)) 
            {
                sss_get_error(error_message,4096,error);
                printf("i=  %"PRIu64 "%s ****************************************\n", i,error_message);
                break;
            }
            if(rows_filled_by_provider<matrix_rows_to_be_fetched) break; // means files ki saari rows read ho gayi coz last batch me ya to matrix ki size ki rows aayegi ya us se km
        }

                if(i%2000 == 0)  // %1000 %500 %500000 if data is 10000000 based on the data length
                {
                 printf("Error : %10.5f\n",error_value);                    
                }
    }
  sss_vector_write_csv(model_vector,"model.csv",error);


goto prepare_for_exit;
    catch_error:
		sss_get_error(error_message,100,error);
		printf("Error is : %s ",error_message);
        sss_vector_destroy(model_vector, NULL);
    prepare_for_exit:
        sss_matrix_destroy(X_matrix, NULL);
        sss_matrix_destroy(XT_matrix, NULL);
        sss_matrix_destroy(matrix,NULL);

        sss_vector_destroy(Y_HAT_vector, NULL);
        sss_vector_destroy(E_vector, NULL);
        sss_vector_destroy(ET_vector, NULL);
        sss_vector_destroy(XTE_vector, NULL);
        sss_vector_destroy(model_vector, NULL);
        sss_vector_destroy(y_vector, NULL);

return;
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
        if(feof(wrapper->file))
        {
            // jb end of li ne nahi diya  in file to ye lagana pdega
            // value=atof(str);
            // sss_matrix_set(matrix,r,c,value,error);

            break;
        } 
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

int main(int c,  char *v[]) {
    sss_err *error;
    sss_data_provider *provider;
    error=sss_error_create_new();

    // variables for gnu plot
            char command[256];
            FILE *plotline, *ploterror;
            sprintf(command, "\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persistent");
            plotline=popen(command,"w");
            ploterror=popen(command,"w");
            if(plotline==NULL || ploterror==NULL)
            {
            perror("popen failed");
            printf("issue\n");
            return 0;
            }
            // Setup gnuplot for line
            fprintf(plotline, "set datafile separator ','\n"); // CSV uses comma
            fprintf(plotline, "set title 'Data + Line'\n");
            fprintf(plotline, "set xlabel 'X'\n");
            fprintf(plotline, "set ylabel 'Y'\n");

            // Setup gnuplot for error
            fprintf(ploterror, "set datafile separator ','\n"); // CSV uses comma
            fprintf(ploterror, "set title 'Error Vs Iterations'\n");
            fprintf(ploterror, "set xlabel 'Iterations'\n");
            fprintf(ploterror, "set ylabel 'Error Value'\n");
    //variables for gnuplot

    // command line args
            uint64_t number_of_iterations;
            char * filename;
            filename =  v[1];
            number_of_iterations=atol(v[2]);
            double learning_rate =  atof(v[3]);
    // command line args

    provider= sss_data_provider_create(filename,0,kalu,error);
    // training file de di ,abhi skip lines 0 coz we dont have header
    if(sss_has_error(error))
    {
        printf("Some issue before calling do something like fit \n");  // error se actual error nikal k print krna
        return 0;
    }
    do_something_like_fit(provider,number_of_iterations,learning_rate,plotline,ploterror,error);
    if(sss_has_error(error))
    {
        
        printf("Some issue after calling someting like fit\n");  // error se actual error nikal k print krna
        return 0;
    }
    // kaam hone k badd desroy provider
    sss_data_provider_destroy(provider,error);
    sss_error_destroy(error);
    pclose(plotline);
    pclose(ploterror);
    return 0;
}