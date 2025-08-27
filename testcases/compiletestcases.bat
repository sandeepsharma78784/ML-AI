cls
del *.out
gcc test_error.c -I ../include -I ../include/error -I ../private/include -I ../include/vector -I ../private/include/error -L ../lib -lsss -o test_error.out
gcc test_matrix.c -I ../include -I ../include/error -I ../include/matrix -I ../include/vector -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_matrix.out
gcc test_matrix_write_read.c -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_matrix_write_read.out
gcc test_read_csv.c -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_read_csv.out
gcc test_vector.c -I ../include -I ../include/error -I ../include/vector -I ../include/vector -I ../private/include -I ../private/include/error -L ../lib -lsss -o test_vector.out

gcc test_vector_fill.c -I ../include -I ../include/error -I ../include/vector  -I ../include/matrix  -L ../lib -lsss -o test_vector_fill.out

REM this is a comment
REM  fit()
REM gcc linear_regression.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o linear_regression.out -Wall

REM  predict()
REM gcc linear_regression_predict.c  -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../private/include -I ../private/include/error -I ../include/math -I ../include/ml_math -L ../lib -lsss -o linear_regression_predict.out -Wall
