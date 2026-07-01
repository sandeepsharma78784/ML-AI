@echo off
setlocal

@REM set PATH=C:\msys64\mingw64\bin;%PATH%
del /Q *.o 2>nul
del /Q ..\lib\libmlcpp.a 2>nul

g++ -I ..\include -c *.cpp
for %%f in (*.o) do (
  ar rcs ..\lib\libmlcpp.a "%%f"
)

echo ..\lib\libmlcpp.a created

endlocal