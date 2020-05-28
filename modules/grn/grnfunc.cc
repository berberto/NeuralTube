/******************************************************************************
 *
 *  grnfunc.cc
 *  
 *  Auxiliary functions for GRN dynamics.
 *
 *  Author: Alberto Pezzotta (alberto.pezzotta [AT] crick.ac.uk)
 *
 *****************************************************************************/

#define GRNFUNC_CC

double Hill (double x)
{
    return x/(1.+x);
}