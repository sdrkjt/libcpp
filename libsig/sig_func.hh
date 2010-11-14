#ifndef sig_func_hh
#define sig_func_hh

#include "AlignedArray.hh"
#include "sig_types.hh"

void sse_real_fc(Sigfl &output, const Sigcxs & input );
OPConv<Sigcxs,Sigfl> real(const Sigcxs & input );

void sse_imag_fc(Sigfl &output, const Sigcxs & input );
OPConv<Sigcxs,Sigfl> imag(const Sigcxs & input );

#endif
