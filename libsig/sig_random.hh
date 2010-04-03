#ifndef sig_random_hh
#define sig_random_hh

#include "AlignedArray.hh"
#include "sig_types.hh"

#ifndef ASM
 #include <math.h>
#else
 #include "asmlibran.h"
 #define random MersenneRandom
#endif

static inline void randomInit(Sigui8 &sout, const int &size){
  sout.SetSize(size);
  for(int ii=0;ii<size;ii++)
    sout[ii] = random()%256;
};


static inline void randomInit(Sigfl &sout, const int &size){
  sout.SetSize(size);
  for(int ii=0;ii<size;ii++)
    sout[ii] = random();
};

static inline  void randomInit(Sigcxs &sout, const int &size){
  sout.SetSize(size);
  for(int ii=0;ii<size;ii++)
    sout[ii] = Cxs(random(),random());

};
static inline  void randomInit(Sigcxd &sout, const int &size){
  sout.SetSize(size);
  for(int ii=0;ii<size;ii++)
    sout[ii] = Cxd(random(),random());
};


#endif
