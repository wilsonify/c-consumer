
mkdir -p build
mkdir -p dist

g++ -c include/cpp-library/cppfunction_.cpp
gcc -c include/c-library/cfunction_.c
f77 -c include/f-library/ffunction.f
f77 -c fprogram.f
f77 -lc -o fprogram.out fprogram.o ffunction.o cppfunction_.o cfunction_.o

mv *.o build
mv *.out dist