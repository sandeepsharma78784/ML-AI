@echo off
setlocal

del /Q *.o 2>nul
del /Q ..\lib\libmlcpp.a 2>nul

g++ -I ..\include -c *.cpp
if errorlevel 1 exit /b %errorlevel%

ar rcs ..\lib\libmlcpp.a *.o
@REM ar rcs ..\lib\libmlcpp.a ml_functions.o
if errorlevel 1 exit /b %errorlevel%

del /Q *.o 2>nul

echo ..\lib\libmlcpp.a created

endlocal

note: we have crreate bat version of create_libmlcpp.sh 
while running we were getting ar command issue 

the issue was it is not picking *.o 
so we have created another bat file 
and used for loop over .o files and added to lib.

to check g++ 
where g++
to check ar
where ar

ar is used to create static lib

note:
    what we are doing see
    delete all .o files
    1. del /Q *.o 2>nul
    delete lib.a file
    2. del /Q ..\lib\libmlcpp.a 2>nul
    compile all cpp files 
    3. g++ -I ..\include -c *.cpp
        ✅ -c
        👉 Compile only (no linking)
        Converts .cpp → .o
        Does NOT create .exe
    then create lib
    4. ar rcs ..\lib\libmlcpp.a *.o
