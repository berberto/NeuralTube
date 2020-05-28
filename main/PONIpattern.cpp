/******************************************************************************
 *
 *	PONIpattern
 *	
 *	Simulating the evolution of an array of cells exposed to a gradient
 *	of Gli, and interpreting it through a PONI network (Cohen et al. '14).
 *
 *	Gives as output the final pattern (protein levels as function of space).
 *
 *	Author: Alberto Pezzotta (alberto.pezzotta [AT] crick.ac.uk)
 *
 *****************************************************************************/

#define MAIN_PROGRAM

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include "random.h"
#include "grn/poni.h"

using namespace Eigen;
using namespace std;


//
//	exponential gradient of effector (Gli)
//
PONI_h_t gliGradient(const double x)
{
	PONI_h_t gli;
	double aux = exp(- x/0.15);
	gli <<	aux, 1.- aux;
	return gli;
}


int main (int argc, char *argv[])
{

	const double dt = .01;	// time discretization
	const double dx = .002;	// lattice spacing

	cout << fixed;
	cout << setprecision(6);

	bool noise = false;		// whether to include low copy-number noise

	// initialize pseudo-random number generator
	if (noise)
	{
		int seed = rlxd_seed();
	    rlxd_init(1,seed);
	}

	// define PONI network variables
	// class defined in ../include/grn/poni.h
	// and implemented in ../modules/grn/poni.cc
	// initialize a PONI object with 				
	PONI start, grn;
	PONI_h_t gli;
	PONI_x_t grnVec;		// vector containing PONI genes
	
	//
	//	SET PARAMETERS
	//
	// use start as a template for all the cells
	// all grn[i] will be set equal to this template at the beginning of the
	// simulation this makes the setup faster, as it requires reading from files

	// write to file the default parameters (Cohen et al. '14)
	start.testParameters("parameters_TEST.dat");

	// use first command line argument as filename with parameters
	// if none is passed, the default are used
	// (the file may contain only a subset of the parameters, others are set to default)
	if (argc == 2) start.setParameters(argv[1]);

	// possible to set parameters individually by name
	// e.g. parameter for strength of the noise (system size)
	start.setParameters("Omega", 500.);

	//
	// SET INITIAL CONDITIONS (ALL CELLS ARE EQUAL)
	//

	grnVec << 	.95,	// Pax6
				0.005,	// Olig2
				0.005,	// Nkx2.2
				.95;	// Irx3
	start.setState(grnVec);
	// equivalent to..
	// start.setState(.95, .005, .005, .95);


	//
	// STEADY STATE FOR PREDOMINANTLY REPRESSIVE INPUT (PREPATTERN CONDITION)
	//
	// set Gli input (constant)
	// Here Gli is passed as two real numbers to the setEffector method
	start.setEffector(0., 1.);

	// simulate system for long time
	for (double t = -1000.; t < 0.; t += dt) {

		// evolve by a step dt (Euler integration)
		// set second variable to 'true' to add noise
		start.evolve(dt, noise);
	}


	//
	// SIMULATION WITH GRADIENT OF GLI
	//
	// loop over all cells (lattice points on a line)
	for (double x = 0.; x < 1.; x += dx)
	{
		// make copies of 'start' at each position on the lattice
		// all cells are equal initially
		grn = start;


		// set Gli constant in a space dependent manner (decreasing GliA, GliR = 1 - GliA)
		// Here Gli is passed as a PONI_h_t variable (Vector2d object from Eigen)
		grn.setEffector(gliGradient(x));
		
		for (double t = 0.; t < 300.; t += dt) {
			// evolve by a step dt (Euler integration)
			// set second variable to 'true' to add noise
			grn.evolve(dt, noise);
		}

		// print final pattern to stdout
		cout << x << "\t" << grn << "\n";

	}

	cout.flush();

	return 0;

}