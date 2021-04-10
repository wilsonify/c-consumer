# tcp-client-server Makefile

CC = gcc
FC = gfortran
FCFLAGS = -Wall -Wextra -std=f2018 -pedantic -fcheck=all

OBJS = mod_dill.o external/libdill.a

.PHONY: all clean
.SUFFIXES: .f90 .o

all: client server

client: client.f90 $(OBJS)
	$(FC) $(FCFLAGS) $< -o $@ external/libdill.a -pthread

server: server.f90 $(OBJS)
	$(FC) $(FCFLAGS) $< -o $@ external/libdill.a -pthread

external/libdill.a: external/libdill-2.14
	cd external/libdill-2.14 && ./configure && make && cp .libs/libdill.a ..

.f90.o:
	$(FC) $(FCFLAGS) -c $<

%.o: %.mod

mod_dill.o: mod_dill.f90

clean:
	$(RM) -rf client server *.o *.mod
