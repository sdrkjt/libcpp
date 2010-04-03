#define check_asm 0
//g++ -S -O3 -msse2 t_complex.cc
#if !check_asm
#include "TSC.hh"
#else
#define TIME(x)
#define DUMP(x)
#define OVERHEAD()
#endif
   
#include <emmintrin.h>
#include <math.h>
#include "../libsig/libsig.hh"



int main(){
  Sigcxs SigIn;
  Sigcxd SigDbIn;
  SigIn.SetSize(8192); SigDbIn.SetSize(8192);
  for(int kk=0;kk<8192;kk++){
    SigIn.val(kk) = Cxs(0.123f + kk,0.456f + kk);
    SigDbIn.val(kk) = Cxd(0.123f + kk,0.456f + kk);
  }
  Sigfl testReal; testReal = real(SigIn);
  Sigfl testImag; testImag = imag(SigIn);
  cout<<testReal<<endl;
  cout<<testImag<<endl;

  // subsig(testsubsig,testReal,start,stop);
  int start = 2, stop = 32;
  Sigfl testsubsig = testReal(start,stop);
  
  testsubsig = testsubsig+testsubsig;

  cout<<testReal<<endl;


  //testReal(1,3) = 1.1110f;
  testsubsig(3,5) = 1.1110f;

  cout<<testReal(0,7)<<endl;
};
