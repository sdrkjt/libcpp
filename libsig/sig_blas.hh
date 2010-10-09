#ifndef sig_blas_hh
#define sig_blas_hh

#include "AlignedArray.hh"
#include "sig_types.hh"


float   sdot_cpp(int n, float  *x,float  *y); 
float   sdot_sse(int   nv, float  *xv, float  *yv);







#endif
