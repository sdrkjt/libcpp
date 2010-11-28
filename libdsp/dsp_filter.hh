#ifndef dsp_filter_hh
#define dsp_filter_hh

#include "libsig.hh"

class dsp_filter{
public:
  int    nbCoef;
  int    nbIput;
  Sigcxs coefIQ;

  Sigcxs dataBuffer;
  Sigcxs coefBuffer_fft;

  Sigfl coefMat[4]; 

  dsp_filter(const Sigfl &coef,int nbIput);
  void restart(){ dataBuffer = 0; }
  void sse_compute(Sigcxs &O_dat,const Sigcxs &I_dat);
  void fft_compute(Sigcxs &O_dat,const Sigcxs &I_dat);
  void cpp_compute(Sigcxs &O_dat,const Sigcxs &I_dat);
};

#endif
 
