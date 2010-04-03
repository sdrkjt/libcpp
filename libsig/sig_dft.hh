#ifndef sig_dft_hh
#define sig_dft_hh

#include "AlignedArray.hh"
#include "sig_types.hh"
#include "sig_eltwise.hh"
#include <fftw3.h>
#include <vector>
using namespace std ;
//#define vector std::vector

template <typename TData ,typename TPlan>
class fftwplan{
public:
  int               fftsize;
  TData *input  ,   *output;
  TPlan planForw,  planBckw;

  static vector< fftwplan > lplan;

  fftwplan(int Size){
    fftwplan *existplan = NULL;
    for(int ii=0;ii<(int)lplan.size();ii++){
      if(lplan[ii].fftsize == Size) existplan = &lplan[ii]; 
    }
    if(existplan==NULL){
      cout<<c_yellow<<"====>Add FFT "<<Size<<c_normal<<endl; 
      fftsize  = Size;
      input    = (TData*) fftwf_malloc(sizeof(TData) * Size);
      output   = (TData*) fftwf_malloc(sizeof(TData) * Size);
      planForw = fftwf_plan_dft_1d(Size, input, output, FFTW_FORWARD,  FFTW_MEASURE);
      planBckw = fftwf_plan_dft_1d(Size, input, output, FFTW_BACKWARD,  FFTW_MEASURE);
      lplan.push_back(*this);
    }
    else{
      fftsize  = Size;
      input    = existplan->input;
      output   = existplan->output;
      planForw = existplan->planForw;
      planBckw = existplan->planBckw;
    }
  }
};

template <typename TData ,typename TPlan>
vector<fftwplan<TData,TPlan> > fftwplan<TData,TPlan>::lplan;


void dftExec(Sigcxs& Sout, const Sigcxs & Sin)	 ;								 
OPConv<Sigcxs,Sigcxs> dft(const Sigcxs & Sin)	 ;
 
void idftExec(Sigcxs& Sout, const Sigcxs & Sin)	 ;								 
OPConv<Sigcxs,Sigcxs> idft(const Sigcxs & Sin)	 ;
 
#endif
