#ifndef dsp_resampler
#define dsp_resampler

#include "libsig.hh"
#include "dsp_fir_design.hh"


class rational_resampler {
public:
  int interp, decima;

  rational_resampler(int interpolation,int decimation,double fractional_bw){
    interp = interpolation;
    decima = decimation;
    
    double beta = 5.0;
    double trans_width = 0.5 - fractional_bw;
    double mid_transition_band = 0.5 - trans_width/2;
    
    Sigfl taps; taps = dsp_fir::low_pass(interpolation,                     // gain
					 1,                                 // Fs
					 mid_transition_band/interpolation, // trans mid point
					 trans_width/interpolation,         // transition width
					 dsp_fir::WIN_KAISER,
					 beta                               // beta
					 );

    int ntaps = taps.size();
    int nzero = ntaps - (ntaps/interp)*interp;
    int newsize = nzero+ntaps;
    Sigfl newtaps; newtaps.zeros(newsize);
    newtaps(nzero,nzero+ntaps-1) = taps;

    Sigfl firList[interp];
    for(int ii=0;ii<interp;ii++)firList[ii].setsize(newsize/interp);

    for(int ii=0;ii<newsize;ii++){
      firList[ii%interp][ii/interp] = newtaps[ii];
    }

    //DUMP(nzero+ntaps);
    //figure(2); curve(1); plot1d(newtaps,"newtaps");mygetchar();
  }

  void cpp_compute(const Sigcxs &I_dat){
    int nbIput  = I_dat.size();
    int nbCoef  = firList[0].size();

    /*copy data to the data buffer*/
    dataBuffer(nbCoef-1,nbCoef+nbIput-2) = I_dat;

    int i = 0; int ifir = 0; int I_pos = 0;
    for(int ii=0;ii<nbOut;ii++){
      O_dat[ii] = cpp_dotproduct_ccf(dataBuffer(I_pos,I_pos+nbCoef-1),firList[ifir]);
      ifir += decima;
      while (ifir >= interp){
	ifir -= interp;
	I_pos++;
      }
    }

    /*prepare data buffer for the next compute*/
    dataBuffer(0,nbCoef-2) = dataBuffer(nbIput,nbCoef+nbIput-2);
  }
};

#endif
