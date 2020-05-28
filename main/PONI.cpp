/******************************************************************************
 *
 *	PONI
 *	
 *	Simulating the evolution of a PONI network (Cohen et al. '14).
 *
 *	Gives as output the time evolution of protein levels.
 *
 *	Author: Alberto Pezzotta (alberto.pezzotta [AT] crick.ac.uk)
 *
 *****************************************************************************/

#define MAIN_PROGRAM

#include <iostream>
#include <cmath>
#include <cstdlib>
#include "random.h"
#include "grn/poni.h"

using namespace Eigen;
using namespace std;


int main (int argc, char *argv[])
{

	const double dt = .01;	// time discretization

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
	PONI grn;
	PONI_h_t gliVec;	// vector containing GliA and GliR
	PONI_x_t grnVec;	// vector containing PONI genes


	//
	//	SET PARAMETERS
	//

	// write to file the default parameters (Cohen et al. '14)
	grn.testParameters("parameters_TEST.dat");

	// use first command line argument as filename with parameters
	// if none is passed, the default are used
	// (the file may contain only a subset of the parameters, others are set to default)
	if (argc == 2) grn.setParameters(argv[1]);

	// possible to set parameters individually by name
	// e.g. parameter for strength of the noise (system size)
	grn.setParameters("Omega", 500.);
	

	//
	// SET INITIAL CONDITIONS
	//

	grnVec << 	.95,	// Pax6
				0.005,	// Olig2
				0.005,	// Nkx2.2
				.95;	// Irx3
	grn.setState(grnVec);


	//
	// STEADY STATE FOR PREDOMINANTLY REPRESSIVE INPUT
	//

	// set Gli input (constant)
	gliVec <<	0.,		// GliA
				1.;		// GliR
	grn.setEffector(gliVec);

	// simulate system for long time
	for (double t = -1000.; t < 0.; t += dt) {

		// evolve by a step dt (Euler integration)
		// set second variable to 'true' to add noise
		grn.evolve(dt, noise);

		// uncomment below to print trajectory to stdout
		// cout << t << "\t" << grn << "\n";
	}


	//
	// SIMULATION WITH PREDOMINANTLY ACTIVATING INPUT
	//

	gliVec <<	1.,		// GliA
				0.;		// GliR
	grn.setEffector(gliVec);
	for (double t = 0.; t < 100.; t += dt) {

		// evolve by a step dt (Euler integration)
		// set second variable to 'true' to add noise
		grn.evolve(dt, noise);

		// uncomment below to print trajectory to stdout
		cout << t << "\t" << grn << "\n";
	}
	cout.flush();

	return 0;

}