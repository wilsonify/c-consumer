FC = gfortran
FCFLAGS = -O3

OBJS = mod_diff.o mod_initial.o

.PHONY: all clean
.SUFFIXES: .f90 .o

all: tsunami

tsunami: tsunami.f90 $(OBJS)
	$(FC) $(FCFLAGS) $< $(OBJS) -o $@

.f90.o:
	$(FC) -c $(FCFLAGS) $<

%.o: %.mod

output.txt: tsunami
	./tsunami > output.txt

water_height_0025.svg: output.txt
	python plot_water_height.py output.txt 25

clean:
	$(RM) tsunami
	$(RM) *.o
	$(RM) *.mod
	$(RM) output.txt
	$(RM) water_height_0025.svg

