
rm -rf build
rm -rf dist
mkdir build
mkdir dist

./compile-cprogram.sh
./compile-cppprogram.sh
./compile-fprogram.sh
