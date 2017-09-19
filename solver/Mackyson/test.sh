HERE=$(dirname $0)

cmake $HERE
make --directory=$HERE
