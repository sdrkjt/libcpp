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
  float fact = 1.0/RAND_MAX;
  for(int ii=0;ii<size;ii++)
    sout[ii] = random()*fact;
};

static inline  void randomInit(Sigcxs &sout, const int &size){
  sout.SetSize(size);
  float fact = 1.0/RAND_MAX;
  for(int ii=0;ii<size;ii++)
    sout[ii] = Cxs(random()*fact,random()*fact);

};
static inline  void randomInit(Sigcxd &sout, const int &size){
  sout.SetSize(size);
  double fact = 1.0/RAND_MAX;
  for(int ii=0;ii<size;ii++)
    sout[ii] = Cxd(random()*fact,random()*fact);
};


#endif
