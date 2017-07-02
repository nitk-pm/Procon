cd ../

if not exist "build" (
    mkdir build
)
cd build
call cmake ../src -G "MinGW Makefiles"
call mingw32-make
cd ../win
