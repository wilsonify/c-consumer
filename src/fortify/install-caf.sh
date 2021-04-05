
sudo apt install openmpi-bin libopenmpi-dev
git clone --branch 2.9.2 https://github.com/sourceryinstitute/OpenCoarrays
cd OpenCoarrays
mkdir build
cd build
FC=gfortran CC=gcc cmake ..
make
make install