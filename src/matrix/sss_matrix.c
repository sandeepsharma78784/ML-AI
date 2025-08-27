#include<private_sss.h>  // here we are using seterror to humkoisko inclde krna pdega for compile time
#include<sss.h>
#include<stdlib.h>


typedef struct __sss_matrix
{
	double *data;
	uint32_t rows;
	uint32_t columns;
}sss_matrix;

sss_matrix * sss_matrix_create_new(uint32_t rows,uint32_t columns,sss_err *error)
{
sss_matrix *matrix;
if(rows==0 && columns==0)
{
	sss_set_error(error,"Invalid row count(0) and column count(0)",SSS_INVALID_DIMENSIONS);
	return NULL;
}
if(rows==0)
{
	sss_set_error(error,"Invalid row count(0)",SSS_INVALID_ROW_COUNT);
	return NULL;
}
if(columns==0)
{
	sss_set_error(error,"Invalid column count(0)",SSS_INVALID_COLUMN_COUNT);
	return NULL;
}
//if all above good means we can create matrix
matrix=(sss_matrix *)malloc(sizeof(sss_matrix));
if(matrix==NULL)
{
	sss_set_error(error,"Low memory",SSS_LOW_MEMORY);
	return NULL;
}
// if matrix is created then allocate memory for actual data
matrix->data=(double *)malloc(sizeof(double)*rows*columns);
if(matrix->data==NULL)
{
	sss_set_error(error,"Low memory",SSS_LOW_MEMORY);
	free(matrix);  // jo memory allocate ho gayi usko free kiya,IMP
	return NULL;
}
matrix->rows=rows;
matrix->columns=columns;
return matrix;
}


void __sss_matrix_destroy(sss_matrix **ptr_2_matrix_ptr, sss_err *error)
{
//here ** liya coz hume user ne jo pointer pass kiya uska address chahiye, 
//jo ki um macro ki help se niche pass krege.
//note: draw the diagram for same in notes
	if(*ptr_2_matrix_ptr==NULL) // means we will get address of pointer that pointing to matrix
	{
	sss_set_error(error,"NULL POINTER",SSS_NULL_POINTER);
	return;
	}
	free((*ptr_2_matrix_ptr)->data);
	free(*ptr_2_matrix_ptr);
	*ptr_2_matrix_ptr=NULL;
}

#define sss_matrix_destroy(j,e) __sss_matrix_destroy(&j,e); //user pass pointer to matrix, and internally we used address of that pointer

void sss_matrix_set(sss_matrix *matrix,uint32_t row,uint32_t column,double value, sss_err *error)
{

char error_message[70]; // we took this coz we are going to use sprintf to prepare message which combine string and variable.
//sprint f ko error_message dege ki yaha likho, and then message + variable in string. sprintf(error_message, "mesag %u heh %u",var1,var2) and so on..


// basic checks: 1. matrix null 2. out of bound 3. -ve : ye to aayega nahi cox -ve converted to +ve coz of uint
if(matrix==NULL)
{
 sss_set_error(error,"NULL POINTER",SSS_NULL_POINTER);
 return;
}

if(row>=matrix->rows && column>matrix->columns)
{
	sprintf(error_message,"Invalid row index (%u) and column index (%u)", row,column);
	sss_set_error(error,error_message,SSS_INVALID_ROW_AND_COLUMN_INDEX);
	return;
}
if(row>=matrix->rows)
{
	sprintf(error_message,"Invalid row index (%u)", row);
	sss_set_error(error,error_message,SSS_INVALID_ROW_INDEX);
	return;
}
if(column>matrix->columns)
{
	sprintf(error_message,"Invalid column index (%u)",column);
	sss_set_error(error,error_message,SSS_INVALID_COLUMN_INDEX);
	return;
}
matrix->data[(row*matrix->columns)+column]=value;  //row major [(rowIndex*columns)+columnIndex]
}

double sss_matrix_get(const sss_matrix *matrix,uint32_t row,uint32_t column,sss_err *error)
{
char error_message[70];
if(matrix==NULL)
{
 sss_set_error(error,"NULL POINTER",SSS_NULL_POINTER);
 return 0.0;
}

if(row>=matrix->rows && column>matrix->columns)
{
	sprintf(error_message,"Invalid row index (%u) and column index (%u)", row,column);
	sss_set_error(error,error_message,SSS_INVALID_ROW_AND_COLUMN_INDEX);
	return 0.0;
}
if(row>=matrix->rows)
{
	sprintf(error_message,"Invalid row index (%u)", row);
	sss_set_error(error,error_message,SSS_INVALID_ROW_INDEX);
	return 0.0;
}
if(column>matrix->columns)
{
	sprintf(error_message,"Invalid column index (%u)",column);
	sss_set_error(error,error_message,SSS_INVALID_COLUMN_INDEX);
	return 0.0;
}
return matrix->data[(row*matrix->columns)+column];
}

 
void sss_matrix_get_dimensions(const sss_matrix *matrix, uint32_t *rows, uint32_t *columns, sss_err *error)
{
	// since we need to return two things that's why we told user to share the mem address so we can write there.
	// and user can read from there
	if(matrix==NULL)
	{
		sss_set_error(error,"NULL POINTER",SSS_NULL_POINTER);
		return;
	}
	if(rows) *rows=matrix->rows; // check ki user jis me copy krwana chahta he vo null to nahi pass kiya
	if(columns) *columns=matrix->columns;
}

uint32_t sss_matrix_get_row_count(const sss_matrix *matrix,sss_err *error)
{
	if(matrix==NULL) 
	{
		sss_set_error(error,"Null Pointer",SSS_NULL_POINTER);
		// return NULL;
	}
	return matrix->rows;
}

uint32_t sss_matrix_get_column_count(const sss_matrix *matrix,sss_err *error)
{
	if(matrix==NULL) 
	{
		sss_set_error(error,"Null Pointer",SSS_NULL_POINTER);
		// return NULL;
	}
	return matrix->columns;
}


void sss_matrix_print(FILE *file, const sss_matrix *matrix,sss_err *error)
{
	uint32_t r,c;
	if(error) sss_clear_error(error);
	if(matrix==NULL)
	{
		sss_set_error(error,"Null Pointer",SSS_NULL_POINTER);
		return;
	}
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;c++)
		{
		fprintf(file, "%12.6lf ",sss_matrix_get(matrix,r,c,NULL)); //humko error nahi lena isliye NULL pass kiya
			//note: yaha file==stdout aaya to monitor pr, and kisi file ka pointer aaya to file me
		}
		fprintf(file,"\n");
	}
}

void sss_matrix_write(const sss_matrix *matrix,const char *filename,sss_err *error)
{
	FILE *file;
	char error_message[4096];
	if(matrix==NULL)
	{
		sss_set_error(error,"Null pointer Argument(1)",SSS_NULL_POINTER);
		return;
	}
	if(filename==NULL)
	{
		sss_set_error(error,"Null pointer Argument(2)",SSS_NULL_POINTER);
		return;
	}
	file=fopen(filename,"wb"); // b for binary mode, on linux no need for b coz text and binary treated in same way
	if(file==NULL)
	{
		sprintf(error_message,"Unable to create file %s",filename);
		sss_set_error(error,error_message,SSS_FILE_WRITE_ERROR); // add this code in error.h
		return;
	}
	// yaha tk pahuche means all corner cases passed
	fwrite(&(matrix->rows),sizeof(uint32_t),1,file);
	fwrite(&(matrix->columns),sizeof(uint32_t),1,file);
	fwrite(matrix->data,sizeof(double)*matrix->rows*matrix->columns,1,file);
	fclose(file);
	
}
sss_matrix * sss_matrix_read(const char *filename, sss_err *error)
{
	sss_matrix *matrix;
	uint32_t rows,columns;
	FILE *f;
	char error_message[4096];
	if(filename==NULL)
	{
		sss_set_error(error,"Null pointer Argument(1)",SSS_NULL_POINTER);
		return NULL;
	}
	f=fopen(filename,"rb");  // read in binary mode
	if(f==NULL)
	{
		sprintf(error_message,"Unable to read file %s",filename);
		sss_set_error(error,error_message,SSS_FILE_READ_ERROR); // add this code in error.h
		return NULL;
	}
	//note: verify the length of file is correct or not depending the contents of first 8 bytes, 
	//first 8 bytes contain info of rows and columns , 4 bytes for each and then actual data start
	fread(&rows,sizeof(uint32_t),1,f);
	fread(&columns,sizeof(uint32_t),1,f);
	// now we read the info of rows and columns lets create matrix
	matrix=sss_matrix_create_new(rows,columns,error);
	if(matrix==NULL)
	{
		fclose(f);
		return NULL;
	}
	//now read the actual data
	fread(matrix->data,sizeof(double)*rows*columns,1,f); //file se read kr k matrix->data ji memory ko point kr raha he usme vo bytes likh dega
	// saare bytes sequentially matrix->data pr copy ho jayege,
	// ek baar me kitne byte krega ye os ki capacity pr depend krega
	fclose(f);
	return matrix;
}
void sss_matrix_write_csv(const sss_matrix *matrix,const char *filename,sss_err *error)
{
FILE *file;
uint32_t r,c;
char error_message[4096];
	if(error) sss_clear_error(error);
	if(matrix==NULL)
	{
		sss_set_error(error,"Nul pointer Argument(1)",SSS_NULL_POINTER);
		return;
	}
	if(filename==NULL)
	{
		sss_set_error(error,"Nul pointer Argument(2)",SSS_NULL_POINTER);
		return;
	}
	file=fopen(filename,"w");
	if(file==NULL)
	{
		sprintf(error_message,"Unable to create file %s", filename);
		sss_set_error(error,error_message,SSS_FILE_WRITE_ERROR); 	//add this error code in header file
		return;
	}
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
		fprintf(file,"%lf",matrix->data[r*matrix->columns +c]);
		if(c!=matrix->columns-1) fputc(',',file);	
		}
		fputc('\n',file);
	
	}
	fclose(file);
}
sss_matrix * sss_matrix_read_csv(const char *filename,uint8_t skip_lines,sss_err *error)
{
	FILE *f;
	char error_message[4096];
	char m,last_character;
	char str[51]; // to read each value separated by commna
	double value; // then convert above arr to double
	sss_matrix *matrix;
	uint32_t rows;
	uint32_t columns;
	long data_starts_at;
	uint32_t previous_row_commas; //to ensure each row hase equal no. of columns means commas
	uint32_t current_row_commas;
	uint32_t r,c;
	int i;

	if(error) sss_clear_error(error); // after declaration we have to clear error is our first task;
	
	//apply checks on incoming parameteres
	if(filename==NULL)
	{
		sss_set_error(error,"Nul pointer Argument(1)",SSS_NULL_POINTER);
		return NULL;
	}
	f=fopen(filename,"r");
	if(f==NULL)
	{
		sprintf(error_message,"Unable to read file %s", filename);
		sss_set_error(error,error_message,SSS_FILE_READ_ERROR); 	//add this error code in header file
		return NULL;
	}

	while(skip_lines>0)
	{
		m=fgetc(f);
		if(feof(f)) break;
		if(m=='\n') skip_lines--;
	}
	if(skip_lines>0)
	{
		sprintf(error_message,"Invalid csv format of file %s",filename);
		sss_set_error(error,error_message,SSS_CSV_FORMAT_ERROR);	//add this error code in header file
		fclose(f);
		return NULL;
	}
	//baat yaha tk pahuchi measn actual data tk pahuche
	data_starts_at=ftell(f);
	previous_row_commas=0;
	current_row_commas=0;
	rows=0;
	while(1)
	{
		m=fgetc(f);
		if(feof(f)) break;
		if(m==',') current_row_commas++;
		if(m=='\n')
		{	//means ek line khtam and iske comma ko store kr lo; and prevois se compare kr lo
			if(previous_row_commas!=0)
			{
			if(previous_row_commas!=current_row_commas)
			{
				sprintf(error_message,"Invalid csv format of file %s",filename);
				sss_set_error(error,error_message,SSS_CSV_FORMAT_ERROR);
				fclose(f);
				return NULL;
			}
			}
			previous_row_commas=current_row_commas;
			current_row_commas=0;  //next row k comma k liye
			rows++;        //yaha tk bat pahuchi mtlb succsfully ek line complete and now f will be at first char of next line
		}
		last_character=m; // at the end of loop isme file ka last char hi aayega and uske baad EOF aayega jiska check first line pr laga he to break the loop
	}
	columns=previous_row_commas+1; // note: dont forget to do +1  , other wise will get invalide row index error;
	if(columns==0)
	{
		sprintf(error_message,"Empty csv file %s",filename);
		sss_set_error(error,error_message,SSS_CSV_EMPTY_ERROR);  //add this error code in header file
		fclose(f);
		return NULL;  
	}

	if(last_character!='\n') rows++;  //means user ne last me enter hit nahi kiya 
	//Note: in case of vim it will add one extra \n while we save the file.
	
	//now we have information of rows and columns lets create a matrix and verify it
	matrix = sss_matrix_create_new(rows,columns,error);
	if(matrix==NULL)
	{
		fclose(f);
		return NULL;
	}

	//now move pointer to start of actual data and start populating file data to matrix
	fseek(f,data_starts_at,0);   //0 start of file, means 0 se start kro and data_start_at tk le jao f ko.
	
	//now read the file
	i=0;
	c=0;
	r=0;
	while(1)
	{
		m=fgetc(f);
		if(feof(f)) break;
		if(m==',' || m=='\n')
		{
			//means ek cell ka data read kr liya
			str[i]='\0';   // terminate with \0 if ek cell ka data aa gaya str me
			i=0;
			value=atof(str); //convert that to double
			sss_matrix_set(matrix,r,c,value,error);
			c++;  //abhi r=0 hi chl rha he and current cell ka data populate kr liya he and to c ko aage bada diya.
			if(c==columns)		//V V IMP
			{
				r++;
				c=0;
				//kitna ideal logic lagaya he 
				// maan lo 5 column he to index bnege 0 1 2 3 4 and yadi c=5 ho jata he means ye row k saare columns me data feeded.
			}
		}
		else  // ye else lgana pdega nahi to str me , and \n bhi read ho jayege and empty csv wala error aa jayega
		{
		str[i++]=m;    // means , and \n k alawa koi char , to usko array me daal do.
		}
		last_character=m;
	}
	//ask sir to put condition for windows if user not hit enter at end of file
	if(last_character!='\n') 
	{
			str[i]='\0';   
			value=atof(str); //convert that to double
			sss_matrix_set(matrix,r,c,value,error);
	}
	//ask sir to put condition for windows if user not hit enter at end of file
	//yaha tk pahuche mtlb eof and successfully populated the matrix;
	fclose(f);
	return matrix;
}


void sss_matrix_transpose(sss_matrix *matrix,sss_err *error)
{
	uint32_t a,b;
	double value;
	double *new_data;
	uint32_t new_rows,new_columns;
	if(error) sss_clear_error(error);
	if(matrix==NULL)
	{
		sss_set_error(error,"Null Pointer",SSS_NULL_POINTER);
		return;
	}
	if(matrix->rows==1 && matrix->columns==1) return;
	if(matrix->rows==1)
	{
		matrix->rows=matrix->columns;
		matrix->columns=1;
		return;
	}
	if(matrix->columns==1)
	{
		matrix->columns=matrix->rows;
		matrix->rows=1;
		return;
	}
	if(matrix->rows==matrix->columns)  //it is a squere matrix
	{
		for(a=0;a<matrix->rows;++a)
		{
			for(b=a+1;b<matrix->columns;++b)
			{
				// to address rowindex*totalcolmnsnumber + colindex
				value=matrix->data[a*matrix->columns+b];	
				matrix->data[a*matrix->columns+b]=matrix->data[b*matrix->columns+a];
				matrix->data[b*matrix->columns+a] = value;
			}
		}
		return;
	}

	new_rows=matrix->columns;
	new_columns=matrix->rows;
	new_data=(double *)malloc(sizeof(double)*new_rows*new_columns);
	if(new_data==NULL)
	{
		sss_set_error(error,"Low memory",SSS_LOW_MEMORY);
		return;
	}
	for(a=0;a<matrix->rows;++a)
	{
	for(b=0;b<matrix->columns;++b)
	{
		new_data[b*new_columns+a] =  matrix->data[a*matrix->columns+b];
	}
	}
	free(matrix->data);
	matrix->data=new_data;
	matrix->rows=new_rows;
	matrix->columns=new_columns;

}
sss_matrix * sss_matrix_transpose_create_new(const sss_matrix *matrix,sss_err *error)
{
	uint32_t a,b;
	sss_matrix *new_matrix;
	if(error) sss_clear_error(error);
	if(matrix==NULL)
	{
		sss_set_error(error,"Null Pointer",SSS_NULL_POINTER);
		return NULL;
	}
	new_matrix=sss_matrix_create_new(matrix->columns,matrix->rows,error); //row column exchange kr diye
	//yaha pr vo column==1 rows==1 wali condition nai lgayi coz copy to krna hi he
	if(new_matrix==NULL) return NULL;
	for(a=0;a<matrix->rows;++a)
	{
	for(b=0;b<matrix->columns;++b)
	{
		new_matrix->data[b*new_matrix->columns+a]=matrix->data[a*matrix->columns+b];	
	}
	}
	return new_matrix;
	//naya wala transpose me bna k data fill kr k return kiya
	
}









// now add the functions  fill(), copy() and get_vector()  functions
void sss_matrix_fill(sss_matrix *input_matrix,uint32_t start_row_index,uint32_t start_column_index,uint32_t end_row_index,uint32_t end_column_index,double fill_value,sss_err *error)
{
	uint32_t r,c;
	if(error) sss_clear_error(error);
	if(input_matrix==NULL)
	{
		sss_set_error(error,"Null pointer",SSS_NULL_POINTER);
		return;
	}
	// check if user pass out of bound end point 
	if(end_row_index>=input_matrix->rows)
	{
		//just rectify and no error throw
		end_row_index=input_matrix->rows-1;
	}
	if(end_column_index>=input_matrix->columns)
	{
		//just rectify and no error throw
		end_column_index=input_matrix->columns-1;
	}
	// we can rectify start point as well if it was out of bound as well
	// now fill the matrix
	for(r=start_row_index;r<=end_row_index;++r)
	{
		for(c=start_column_index;c<=end_column_index;++c)
		{
			input_matrix->data[r*input_matrix->columns+c]=fill_value;
		}
	}
}
void sss_matrix_copy(sss_matrix *target,uint32_t target_start_row_index,uint32_t target_start_column_index,const sss_matrix *source,uint32_t source_start_row_index,uint32_t source_start_column_index,uint32_t source_end_row_index,uint32_t source_end_column_index,sss_err *error)
{
	uint32_t target_r,target_c;
	uint32_t source_r,source_c;
	uint32_t target_end_row_index,target_end_column_index; // this we will calculate
	if(error) sss_clear_error(error);
	if(target==NULL)
	{
		sss_set_error(error,"Null pointer argument(1)",SSS_NULL_POINTER);
		return;
	}
	if(source==NULL)
	{
		sss_set_error(error,"Null pointer argument(2)",SSS_NULL_POINTER);
		return;
	}
	
	// check target matrix pick point [start point] is inbound
	if(target_start_row_index>=target->rows)
	{
		return;
	} 
	if(target_start_column_index>=target->columns)
	{
		return;
	}
	
	//check source matrix endpoint is inbound
	if(source_end_row_index>=source->rows)
	{
		source_end_row_index=source->rows-1;
	}
	if(source_end_column_index>=source->columns)
	{
		source_end_column_index=source->columns-1;
	}

	//now set the targe end point
	target_end_row_index=target_start_row_index + (source_end_row_index-source_start_row_index);
	target_end_column_index=target_start_column_index + (source_end_column_index-source_start_column_index);
	

	// check if size of target is not enough to copy all element of source
	// means jitna copy krne ko bola uske hisab se jo end point bna vo target ki (rows-1,columns-1) point k andr he ya nahi
	if(target_end_row_index>=target->rows) // means is row index is crossing the size
	{
		target_end_row_index=target->rows-1;
	}
	if(target_end_column_index>=target->columns) // means is row index is crossing the size
	{
		target_end_column_index=target->columns-1;
	}

source_r=source_start_row_index;
for(target_r=target_start_row_index;target_r<=target_end_row_index;++target_r)
{
	source_c=source_start_column_index;
	// now we have r,c for source and r for target lets create the c for target in loop declaration
	for(target_c=target_start_column_index;target_c<=target_end_column_index;++target_c)
	{
		target->data[target_r*target->columns+target_c]	= source->data[source_r*source->columns+source_c];
		++source_c;	
	}
	++source_r;
}	

// loop ends copy done , we can declare source_r as well in outer loop declaration part
// and same for increment of source_C and source_R in loop , but we did in separately in while() PATTERN


}
sss_vector * sss_matrix_get_vector(const sss_matrix *source,uint32_t source_start_row_index,uint32_t source_start_column_index,uint32_t source_end_row_index,uint32_t source_end_column_index,sss_err *error)
{
	uint32_t r,c,i;
	char error_message[4096];
	uint32_t vector_size;	// we will caculate it based on start and end point from source
	sss_vector *vector;	// we will fill and return this
	uint32_t rows_to_pick,columns_to_pick;
	if(error) sss_clear_error(error);
	if(source==NULL)
	{
		sss_set_error(error,"Null pointer",SSS_NULL_POINTER);
		return NULL;
	}
	// check if source start point is out of bound  and return an error message
	if(source_start_row_index>=source->rows)
	{
		 sprintf(error_message,"Cannot create vector using (%u,%u)-(%u,%u)",source_start_row_index,source_start_column_index,source_end_row_index,source_end_column_index);
		 sss_set_error(error,error_message,SSS_GET_VECTOR_ERROR);
		return NULL;
	}
	if(source_start_column_index>=source->columns)
	{
		 sprintf(error_message,"Cannot create vector using (%u,%u)-(%u,%u)",source_start_row_index,source_start_column_index,source_end_row_index,source_end_column_index);
		 sss_set_error(error,error_message,SSS_GET_VECTOR_ERROR);
		return NULL;
	}

	// similarly check if source end point is out of bound  and return an error message
	if(source_end_row_index>=source->rows)
	{
		 sprintf(error_message,"Cannot create vector using (%u,%u)-(%u,%u)",source_start_row_index,source_start_column_index,source_end_row_index,source_end_column_index);
		 sss_set_error(error,error_message,SSS_GET_VECTOR_ERROR);
		return NULL;
	}
	if(source_end_column_index>=source->columns)
	{
		 sprintf(error_message,"Cannot create vector using (%u,%u)-(%u,%u)",source_start_row_index,source_start_column_index,source_end_row_index,source_end_column_index);
		 sss_set_error(error,error_message,SSS_GET_VECTOR_ERROR);
		return NULL;
	}


	//if asked start and end point are well 
	// then calculate the number of rows to pick and number of columns to pick
rows_to_pick=source_end_row_index-source_start_row_index+1;
columns_to_pick=source_end_column_index-source_start_column_index+1;

//again check if it is forming a vector or a matrix
// kisi bhi ek ka size 1 hona chahiye , so
if(rows_to_pick>1 && columns_to_pick>1)
{
	sprintf(error_message,"Cannot create vector using (%u,%u)-(%u,%u)",source_start_row_index,source_start_column_index,source_end_row_index,source_end_column_index);
	sss_set_error(error,error_message,SSS_GET_VECTOR_ERROR);
		return NULL;
}

//now set the size
if(rows_to_pick==1) vector_size==columns_to_pick;
else vector_size = rows_to_pick;

// now create the vector
vector=sss_vector_create_new(vector_size,error);
if(vector==NULL) return NULL;

if(rows_to_pick==1)
{
// source k r common rahega and c++ hoga 
// and udr vector k liye i le lege 
	r=source_start_row_index;
	i=0;
	for(c=source_start_column_index;c<=source_end_column_index;++c)
	{
		sss_vector_set(vector,i,source->data[r*source->columns+c],error);
		// formula for column major is rindex * total colmns +cindex
		++i;
	}
}
else
{
// source k c common rahega and r++ hoga
	c=source_start_column_index;
	i=0;
	for(r=source_start_row_index;r<=source_end_row_index;++r)
	{
		sss_vector_set(vector,i,source->data[r*source->columns+c],error);
		// formula for column major is rindex * total colmns +cindex
		++i;
	}

}

// finally return the vector
return vector;
}


double * sss_matrix_get_data(const sss_matrix *matrix, sss_err *error)
{
	if(error) sss_clear_error(error);
	if(matrix==NULL)
	{
		sss_set_error(error,"Null pointer",SSS_NULL_POINTER);
		return NULL;
	}
	return matrix->data;
}



