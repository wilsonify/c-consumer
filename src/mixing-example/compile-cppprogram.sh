

mkdir -p build
mkdir -p dist

gcc -c include/c-library/cfunction.c
f77 -c include/f-library/ffunction.f
g++ -c include/cpp-library/cppfunction.cpp
g++ -c cppprogram.cpp
g++ -o cppprogram.out cppprogram.o cfunction.o cppfunction.o ffunction.o

mv *.o build
mv *.out dist