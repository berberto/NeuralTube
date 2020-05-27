#define PONINET_CC

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <Eigen/Dense>
#include "random.h"
#include "grn/poni.h"

using namespace std;
using namespace Eigen;



/*
 *    ####    ###   ####    ###   #   #  #####  #####  #####  ####    #### 
 *    #   #  #   #  #   #  #   #  ## ##  #        #    #      #   #  #     
 *    ####   #####  ####   #####  # # #  ####     #    ####   ####    ###   
 *    #      #   #  #  #   #   #  #   #  #        #    #      #  #       # 
 *    #      #   #  #   #  #   #  #   #  #####    #    #####  #   #  ####
 */

//
//  Set default values of the parameters
//
void PONI::defaultParameters()
{
    pars = {
        {"lambdaConc",1},
        {"lambdaTime",1},
        {"K_Pol_Pax", 4.8},     
        {"K_Pol_Oli", 47.8},    
        {"K_Pol_Nkx", 27.4},    
        {"K_Pol_Irx", 23.4},    
        {"K_Gli_Oli", 18.0},    
        {"K_Gli_Nkx", 373.},    
        {"K_Oli_Pax", 1.9},     
        {"K_Oli_Nkx", 27.1},    
        {"K_Nkx_Oli", 60.6},    
        {"K_Pax_Nkx", 4.8},     
        {"K_Nkx_Pax", 26.7},    
        {"K_Irx_Oli", 28.4},    
        {"K_Oli_Irx", 58.8},    
        {"K_Irx_Nkx", 47.1},    
        {"K_Nkx_Irx", 76.2},    
        {"f_A", 10.},           
        {"C_Pol", .8},          
        {"alpha_Pax", 2.},      
        {"alpha_Oli", 2.},      
        {"alpha_Nkx", 2.},      
        {"alpha_Irx", 2.},      
        {"delta", 2.},          
        {"Omega", 1000.},       
    };
}

//
//  Assign the values in the unordered map
//  to the corresponding real variable
//
void PONI::assignParameters()
{
    lambdaConc  = pars["lambdaConc"];
    lambdaTime  = pars["lambdaTime"];
    K_Pol_Pax   = pars["K_Pol_Pax"]  / lambdaConc;
    K_Pol_Oli   = pars["K_Pol_Oli"]  / lambdaConc;
    K_Pol_Nkx   = pars["K_Pol_Nkx"]  / lambdaConc;
    K_Pol_Irx   = pars["K_Pol_Irx"]  / lambdaConc;
    K_Gli_Oli   = pars["K_Gli_Oli"]  / lambdaConc;
    K_Gli_Nkx   = pars["K_Gli_Nkx"]  / lambdaConc;
    K_Oli_Pax   = pars["K_Oli_Pax"]  / lambdaConc;
    K_Oli_Nkx   = pars["K_Oli_Nkx"]  / lambdaConc;
    K_Nkx_Oli   = pars["K_Nkx_Oli"]  / lambdaConc;
    K_Pax_Nkx   = pars["K_Pax_Nkx"]  / lambdaConc;
    K_Nkx_Pax   = pars["K_Nkx_Pax"]  / lambdaConc;
    K_Irx_Oli   = pars["K_Irx_Oli"]  / lambdaConc;
    K_Oli_Irx   = pars["K_Oli_Irx"]  / lambdaConc;
    K_Irx_Nkx   = pars["K_Irx_Nkx"]  / lambdaConc;
    K_Nkx_Irx   = pars["K_Nkx_Irx"]  / lambdaConc;
    f_A         = pars["f_A"];
    C_Pol       = pars["C_Pol"]      * lambdaConc;
    alpha_Pax   = pars["alpha_Pax"]  * lambdaConc / lambdaTime;
    alpha_Oli   = pars["alpha_Oli"]  * lambdaConc / lambdaTime;
    alpha_Nkx   = pars["alpha_Nkx"]  * lambdaConc / lambdaTime;
    alpha_Irx   = pars["alpha_Irx"]  * lambdaConc / lambdaTime;
    delta       = pars["delta"]      / lambdaTime;
    Omega       = pars["Omega"];
}


//  Set one parameter through its key (string)
void PONI::setParameters(string key, double val)
{
    auto it = pars.find(key);
    if(it != pars.end())
        it->second = val;
    else
    {
        cout << "error: setParameters (PONI): invalid parameter name \""
             << key << "\"\n";
        exit(EXIT_FAILURE);
    }

    // once the map has been updated according to the file,
    // update the values of the corresponding parameters
    assignParameters();
}


//  Set a number of parameters contained into a file in "key  value" form
void PONI::setParameters(const char* filename)
{
    ifstream parf(filename);
    string key;
    double val;
    while (parf >> key >> val)
    {
        auto it = pars.find(key);
        if(it != pars.end())
            it->second = val;
        else
        {
            cout << "error: setParameters: invalid parameter name in \""
                 << filename << "\":  " << key << "\n";
            exit(EXIT_FAILURE);
        }
    }

    // once the map has been updated according to the file,
    // update the values of the corresponding parameters
    assignParameters();
}


void PONI::testParameters(const char* filename)
{
    ofstream os;
    os.open(filename, ios::out);
    this->testParameters(os);
    os.close();
}


void PONI::testParameters(ostream& os)
{
    os << "# PONI network parameters\n";
    for( const auto& n : pars )
    {
        os << n.first << "\t\t" << n.second << "\n";
    }
    os << "\n";
}


/*
 *     ###   ###   #   #   ###  #####  ####   
 *    #     #   #  ##  #  #       #    #   #  
 *    #     #   #  # # #   ##     #    ####   
 *    #     #   #  #  ##     #    #    #  #    ##
 *     ###   ###   #   #  ###     #    #   #   ##
 */
PONI::PONI (){
    x << 0., 0., 0., 0.;
    h << 0., 0.;
    defaultParameters();
    assignParameters();
}

PONI::PONI(double x0, double x1, double x2, double x3)
{
    x << x0, x1, x2, x3;
    h << 0., 0.;
    defaultParameters();
    assignParameters();
}

PONI::PONI(double x0, double x1, double x2, double x3, double eff1, double eff2)
{
    x << x0, x1, x2, x3;
    h << eff1, eff2;
    defaultParameters();
    assignParameters();
}

PONI::PONI(PONI_x_t vec)
{
    x = vec;
    h << 0., 0.;
    defaultParameters();
    assignParameters();
}

PONI::PONI(PONI_x_t vec, PONI_h_t eff)
{
    x = vec;
    h = eff;
    defaultParameters();
    assignParameters();
}

// copy constructor
PONI::PONI (const PONI & b)
{
    x = b.x;
    h = b.h;
    pars = b.pars;
    assignParameters();
}

// assignment 
PONI& PONI::operator= (const PONI & b)
{
    x = b.x;
    h = b.h;
    pars = b.pars;
    assignParameters();
    return *this;
}


/*
 *    #   #  ####  #####  #   #   ###   ####    ###  
 *    ## ##  #       #    #   #  #   #  #   #  #     
 *    # # #  ###     #    #####  #   #  #   #   ##   
 *    #   #  #       #    #   #  #   #  #   #     #  
 *    #   #  ####    #    #   #   ###   ####   ###   
 */

void PONI::setState(double x0, double x1, double x2, double x3)
{
    x << x0, x1, x2, x3;
}

void PONI::setState(PONI_x_t vec)
{
    x = vec;
}

void PONI::setEffector(double eff1, double eff2)
{
    h << eff1, eff2;
}

void PONI::setEffector(PONI_h_t eff)
{
    h = eff;
}


ostream& operator<< (ostream& os, const PONI& vec)
{
    os << (vec.x).transpose() << "\t" << (vec.h).transpose();
    return os;
}

PONI_x_t PONI::getState () const
{
    return x;
}

PONI_h_t PONI::getEffector () const
{
    return h;
}

PONI_x_t PONI::getDrift ()
{
    setDrift();
    return drift;
}

void PONI::setProdR ()
{
    Z   << 0., 0., 0., 0.;
    phi << 0., 0., 0., 0.;

    double aux_1, aux_2, aux_3, aux_4, aux_5;

    // Pax
    aux_1 = 1. + K_Oli_Pax * x(1); // 1 + K_Oli_Pax * [O]
    aux_2 = 1. + K_Nkx_Pax * x(2); // 1 + K_Nkx_Pax * [N]
    Z(0) = K_Pol_Pax * C_Pol \
           + aux_1*aux_1 * aux_2*aux_2;
    phi(0) = K_Pol_Pax * C_Pol / Z(0);

    // Olig
    aux_1 = 1. + K_Nkx_Oli * x(2);             // 1 + K_Nkx_Oli * [N]
    aux_2 = 1. + K_Irx_Oli * x(3);             // 1 + K_Irx_Oli * [I]
    aux_3 = 1. + f_A * K_Gli_Oli * h(0);       // 1 + c_AP * K_Gli_Oli * [A]
    aux_4 = 1. + K_Gli_Oli * ( h(0) + h(1) );  // 1. + K_Gli_Oli * ([A] + [R])
    Z(1) = K_Pol_Oli * C_Pol * aux_3 \
           + aux_4 * aux_1*aux_1 * aux_2*aux_2;
    phi(1) = K_Pol_Oli * C_Pol * aux_3 / Z(1);

    // Nkx
    aux_1 = 1. + K_Oli_Nkx * x(1);             // 1 + K_Oli_Nkx * [O]
    aux_2 = 1. + K_Irx_Nkx * x(3);             // 1 + K_Irx_Nkx * [I]
    aux_3 = 1. + K_Pax_Nkx * x(0);             // 1 + K_Pax_Nkx * [P]
    aux_4 = 1. + f_A * K_Gli_Nkx * h(0);       // 1 + c_AP * K_Gli_Nkx * ([A] + [R])
    aux_5 = 1. + K_Gli_Nkx * ( h(0) + h(1) );  // 1 + K_Gli_Nkx * ([A] + [R])
    Z(2) = K_Pol_Nkx * C_Pol * aux_4 \
           + aux_5 * aux_1*aux_1 * aux_2*aux_2 * aux_3*aux_3;
    phi(2) = K_Pol_Nkx * C_Pol * aux_4 / Z(2);

    // Irx
    aux_1 = 1. + K_Oli_Irx * x(1); // 1 + K_Oli_Pax * [O]
    aux_2 = 1. + K_Nkx_Irx * x(2); // 1 + K_Nkx_Pax * [N]
    Z(3) = K_Pol_Irx * C_Pol \
           + aux_1*aux_1 * aux_2*aux_2;
    phi(3) = K_Pol_Irx * C_Pol / Z(3);

}


PONI_x_t PONI::getPartFunc()
{
    return Z;
}

PONI_x_t PONI::getProdR()
{
    return phi;
}


// force
void PONI::setDrift ()
{
    setProdR();
    drift(0) = alpha_Pax * phi(0) - delta * x(0);
    drift(1) = alpha_Oli * phi(1) - delta * x(1);
    drift(2) = alpha_Nkx * phi(2) - delta * x(2);
    drift(3) = alpha_Irx * phi(3) - delta * x(3);
}

// noise vector
void PONI::setNoise ()
{    
    double g[4];
    gauss_dble(g,4);
    noise(0) = sqrt(alpha_Pax * phi(0) + delta * x(0))*g[0];
    noise(1) = sqrt(alpha_Oli * phi(1) + delta * x(1))*g[1];
    noise(2) = sqrt(alpha_Nkx * phi(2) + delta * x(2))*g[2];
    noise(3) = sqrt(alpha_Irx * phi(3) + delta * x(3))*g[3];
}


void PONI::evolve (double dt, bool stoch)
{
    PONI_x_t xp, xpp;

    setDrift();       // this also sets the necessary variables for noise
    xpp = x + drift * dt;
    xp = xpp;
    if (stoch) {
        do {
            setNoise();
            xp = xpp + sqrt(dt/Omega) * noise;
        } while ( (xp(0) < 0.) || (xp(1) < 0.) || (xp(2) < 0.) || (xp(3) < 0.) );
    }
    x = xp;
}