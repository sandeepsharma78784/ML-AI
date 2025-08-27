1. typedef struct __sss_matrix
    {
        double *data;
        uint32_t rows;
        uint32_t columns;
    }sss_matrix;
    sss_matrix *matrix;
    if this is defined in a file then : matrix->rows, matrix->columns, matrix->data is valid
    but outside this file it is not valid
    so to acces this outside we need to write a mediator function in the same file where this struct is defined 
    that why we need to define functions like 
    sss_matrix_get_dimensions(), sss_matrix_get_row_count(), sss_matrix_get_column_count(); 
    and since data is for private so we defined sss_matrix_get_data() into private header file.




to create and compile the lib move to path  and run create_lib.bat script:
C:\ml-framework\obj-files>create_lib.bat 

to test the lib , create a test file with .c write code and add the line to compile in 
compiletestcases.bat file and then to compile all files in testcases folder run the script compiletestcases.bat
C:\ml-framework\testcases>compiletestcases.bat
