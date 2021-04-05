FC = gfortran
FCFLAGS = -O3

.PHONY: all clean

all: tsunami

tsunami: tsunami.f90
	$(FC) $(FCFLAGS) $< -o $@

clean:
	$(RM) tsunami
	$(RM) output.txt
	$(RM) water_height_ch02.svg
