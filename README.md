# Neuraltube

### Simulations of signalling and GRN for ventral patterning of the vertebrate neural tube

- `include/`: definitions of C/C++ functions and classes 
- `modules/`: implementation of functions and classes
- `main/`: executables and `Makefile`

#### Dependencies

- `gcc` with C++11 support (`gcc > 4.8.1`) -- only tested on UNIX.
- `Eigen`, [here](http://eigen.tuxfamily.org/index.php?title=Main_Page) -- it can be copied either inside `include/` or `/usr/include`; if copied elsewhere, add its path to `INCPATH` variable in `Makefile`


### Usage

Usage of the main functions of the `PONI` class exemplified in `main/PONI.cpp`.

#### To compile and run:

```bash
	cd main
	make
	./PONI [parameter file (optional)] > [output_file]
```

One example of file correctly formatted to pass parameters is `parameters_PONI.dat`.
The method `testParameters` of the `PONI` class prints current values of each parameter and shows the correct name to be used in the file.