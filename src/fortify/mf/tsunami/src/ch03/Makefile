FC = gfortran
FCFLAGS = -O3

.PHONY: all clean

all: tsunami output.txt water_height_0025.svg

tsunami: tsunami.f90
	$(FC) $(FCFLAGS) $< -o $@

output.txt: tsunami
	./tsunami > output.txt

water_height_0025.svg: output.txt
	python plot_water_height.py output.txt 25

clean:
	$(RM) tsunami
	$(RM) output.txt
	$(RM) water_height_0025.svg
