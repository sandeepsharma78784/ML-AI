cls
del *.out
REM since we are including matrix, vector, math in sss.h and also we are including sss.h in test_error.c so we need to specipy path for matrix, vector , math all these three to 
REM to compile this test file even we don't used function from matrix, vector , math in this test file
REM gcc test_error.c -I ../include -I ../include/error -I ../private/include -I ../include/vector -I ../include/matrix  -I ../include/math  -I ../private/include/error -L ../lib -lsss -o test_error.out

REM gcc test_matrix.c -I ../include -I ../include/error -I ../include/matrix -I ../include/vector -I ../include/math  -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_matrix.out
REM gcc test_matrix_write_read.c -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../include/math  -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_matrix_write_read.out
REM gcc test_read_csv.c -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../include/math  -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_read_csv.out
REM gcc test_vector.c -I ../include -I ../include/error -I ../include/vector -I ../include/vector -I ../include/math  -I ../include/matrix -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_vector.out
REM gcc test_vector_fill.c -I ../include -I ../include/error -I ../include/vector  -I ../include/matrix -I ../include/math   -L ../lib -lsss -o test_vector_fill.out


REM  fit()
REM gcc linear_regression.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix  -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o linear_regression.out -Wall

REM  predict()
 gcc linear_regression_predict.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o linear_regression_predict.out -Wall
 
REM gcc linear_regression.c   -I ../include -I ../include/error -I ../include/vector -I ../include/matrix  -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o linear_regression.out -Wall
REM gcc linear_regression_predict.c   -I ../include -I ../include/error -I ../include/vector -I ../include/matrix  -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o linear_regression_predict.out -Wall
gcc data_provider.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix  -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o data_provider.out -Wall
gcc linear_regression_with_gnuplot.c   -I ../include -I ../include/error -I ../include/vector -I ../include/matrix  -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o linear_regression_with_gnuplot.out -Wall
REM gcc SGD_MBGD_with_DataProvider.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix  -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o SGD_MBGD_with_DataProvider.out -Wall
gcc SGD_MBGD_with_GNUPlot.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix  -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o SGD_MBGD_with_GNUPlot.out -Wall
