echo "clean"
rm -rf build
rm -rf dist
mkdir build
mkdir dist
cd build || exit
echo "working directory = $(pwd)"

echo "start conan"
conan install ..
echo "done conan"

echo "start cmake"
cmake ..
echo "done cmake"

echo "start make"
make
echo "done make"

echo "move binary"
mv bin/* ../dist
echo "binary is located in dist"
