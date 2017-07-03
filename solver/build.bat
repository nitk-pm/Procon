
@echo off

if not "%1" == "" if "%1" == "clean" (
    rm -Rf build
    rm -Rf bin
)

if not exist "build" (
    mkdir build
)
cd build
call cmake ../ -G "MinGW Makefiles"
call mingw32-make
cd ../