

mkdir -p build
mkdir -p dist

f77 -c include/f-library/ffunction.f
g++ -c include/cpp-library/cppfunction-extern.cpp
gcc -c include/c-library/cfunction.c
gcc -c cprogram.c
gcc -o cprogram.out cprogram.o ffunction.o cppfunction-extern.o cfunction.o

mv *.o build
mv *.out dist