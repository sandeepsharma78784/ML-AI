cls
del *.o
gcc -c  ../src/error/*.c -I ../include -I ../include/error -I ../include/vector -I ../include/matrix -I ../include/math
gcc  -c  ../src/matrix/*.c -I ../include -I ../include/error -I ../include/matrix -I ../private/include -I ../include/vector -I ../include/math
gcc -c  ../src/vector/*.c -I ../include -I ../include/error -I ../include/vector  -I ../include/matrix -I ../private/include -I ../include/math

gcc -c  ../src/math/*.c -I ../include -I ../include/error -I ../include/vector  -I ../include/matrix -I ../private/include -I ../private/include/error -I ../private/include/vector -I ../private/include/matrix -I ../include/math
gcc -c  ../src/ml_math/*.c -I ../include -I ../include/error -I ../include/vector  -I ../include/matrix -I ../include/math -I ../private/include -I ../private/include/error -I ../private/include/vector -I ../private/include/matrix -lm -Wall

del ..\lib\libsss.a
ar rcs ../lib/libsss.a *.o
