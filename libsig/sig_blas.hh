#ifndef sig_blas_hh
#define sig_blas_hh

#include "AlignedArray.hh"
#include "sig_types.hh"


float   sdot_cpp(const Sigfl &x, const Sigfl &y); 
float   sdot_sse(const Sigfl &xv, const Sigfl &yv);







#endif
