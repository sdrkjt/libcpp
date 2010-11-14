#ifndef sig_blas_hh
#define sig_blas_hh

#include "AlignedArray.hh"
#include "sig_types.hh"


float   cpp_dotproduct_fff(const Sigfl &x, const Sigfl &y); 
float   sse_dotproduct_fff(const Sigfl &x, const Sigfl &y);

Cxs     cpp_dotproduct_ccf(const Sigcxs &x, const Sigfl &y);
Cxs     sse_dotproduct_ccf(const Sigcxs &x, const Sigfl &y);


/********functions that use dotproduct*********************/
float meanAbs2(const Sigfl  &x);
float meanAbs2(const Sigcxs &x);
//double meanAbs2(const Sigcxd &x);

#endif
