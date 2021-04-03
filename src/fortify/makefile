PROGRAM=strategies
FC := /usr/bin/gfortran # /usr/bin/f77
FCFLAGS = # -fdefault-real-8 -fbacktrace -fno-align-commons -fbounds-check
FFFLAGS =
.PHONY: paths all clean test

$(PROGRAM).out: $(PROGRAM).o
	$(FC) $(FFFLAGS) $^ -o $@

$(PROGRAM).o: $(PROGRAM).f
	$(FC) -c $(FCFLAGS) $^

all: $(PROGRAM).out

clean:
	@echo Cleaning object files
	@rm -rf *.o
	@echo Cleaning binary files
	@rm -rf *.out
	@echo Cleaning module files
	@rm -rf *.mod