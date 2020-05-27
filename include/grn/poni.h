#ifndef PONINET_H
#define PONINET_H

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <utility>
#include <tuple>
#include <unordered_map>
#include <Eigen/Dense>
#include "grn/global.h"

using namespace std;
using namespace Eigen;

#ifndef NET_TYPE_DEFS
#define NET_TYPE_DEFS
typedef unordered_map<string,int> NET_dict;
typedef unordered_map<string,double> PAR_dict;
#endif

typedef Vector4d PONI_x_t;  // type of state variable
typedef Vector2d PONI_h_t;  // type of effector variable


class PONI {

private:

    PAR_dict pars;  // dictionary containing parameters (keys are names)

    void defaultParameters();
    void assignParameters();

    double lambdaConc;
    double lambdaTime;
    //
    // BINDING AFFINITIES
    //
    // read: K_A_B = binding of A onto B
    // Affinities RNA-polymerase to P,O,N,I
    double K_Pol_Pax;
    double K_Pol_Oli;
    double K_Pol_Nkx;
    double K_Pol_Irx;

    // Affinities of Gli to O,N
    double K_Gli_Oli;
    double K_Gli_Nkx;

    // Affinities of P,O,N,I to themselves (all pairs).
    double K_Oli_Pax;

    double K_Oli_Nkx;
    double K_Nkx_Oli;

    double K_Pax_Nkx;
    double K_Nkx_Pax;

    double K_Irx_Oli;
    double K_Oli_Irx;

    double K_Irx_Nkx;
    double K_Nkx_Irx;

    // Activator -- RNAp binding cooperativity
    double f_A;

    // Concentration RNA-polymerase
    double C_Pol;

    // Coefficient production rate
    double alpha_Pax;
    double alpha_Oli;
    double alpha_Nkx;
    double alpha_Irx;

    // Degradation rate
    double delta;

    // "protein copy number" (inverse noise strength)
    double Omega;


    NET_dict dict = {
        {"Pax", 0},
        {"Oli", 1},
        {"Nkx", 2},
        {"Irx", 3}
    };


    PONI_x_t x;    // state variables
    PONI_h_t h;    // control/effector variables

    PONI_x_t Z;    // partition function
    PONI_x_t phi;  // probability of RNA-polimerase bound

    PONI_x_t drift;
    PONI_x_t noise;
    
    void setProdR ();
    void setDrift ();
    void setNoise ();

public:

    // contructors
    PONI ();
    PONI (double x0, double x1, double x2, double x3);
    PONI (double x0, double x1, double x2, double x3, double eff1, double eff2);
    PONI (PONI_x_t vec);
    PONI (PONI_x_t vec, PONI_h_t eff);

    PONI (const PONI & b);            // copy-constructor
    PONI& operator= (const PONI & b); // assignment

    void setParameters(string key, double val);
    void setParameters(const char* filename);
    void testParameters(const char* filename);
    void testParameters(ostream& os);

    void setState (double x0, double x1, double x2, double x3);
    void setState (PONI_x_t vec);
    void setEffector(double eff1, double eff2);
    void setEffector(PONI_h_t eff);

    friend ostream& operator<< (ostream& os, const PONI& vec);

    PONI_x_t getState () const;
    PONI_h_t getEffector () const;

    PONI_x_t getDrift();

    PONI_x_t getProdR ();

    PONI_x_t getPartFunc ();
    
    void evolve (double dt, bool stoch);

};


#endif