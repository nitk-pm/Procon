if not exist "build" (
    mkdir build
)
cd build
call cmake ../ -G "MinGW Makefiles"
call mingw32-make
cd ../
