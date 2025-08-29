cls
del *.out
REM since we are including matrix, vector, math in sss.h and also we are including sss.h in test_error.c so we need to specipy path for matrix, vector , math all these three to 
REM to compile this test file even we don't used function from matrix, vector , math in this test file
gcc test_error.c -I ../include -I ../include/error -I ../private/include -I ../include/vector -I ../include/matrix  -I ../include/math  -I ../private/include/error -L ../lib -lsss -o test_error.out

gcc test_matrix.c -I ../include -I ../include/error -I ../include/matrix -I ../include/vector -I ../include/math  -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_matrix.out
gcc test_matrix_write_read.c -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../include/math  -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_matrix_write_read.out
gcc test_read_csv.c -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../include/math  -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_read_csv.out
gcc test_vector.c -I ../include -I ../include/error -I ../include/vector -I ../include/vector -I ../include/math  -I ../include/matrix -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_vector.out
gcc test_vector_fill.c -I ../include -I ../include/error -I ../include/vector  -I ../include/matrix -I ../include/math   -L ../lib -lsss -o test_vector_fill.out


REM  fit()
REM gcc linear_regression.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix  -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o linear_regression.out -Wall

REM  predict()
REM gcc linear_regression_predict.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o linear_regression_predict.out -Wall
 
gcc data_provider.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix  -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o data_provider.out -Wall
