HERE=$(dirname $0)

cd $HERE

cmake -DCMAKE_CXX_COMPILER=g++-7 .
make
