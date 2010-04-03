#ifndef dvbgen_ofdm_hh
#define dvbgen_ofdm_hh
#include "libsig.hh"
#include "dvbgen_reference_signal.hh"

class OfdmWaveForm
{
public:
  OfdmWaveForm();
  ~OfdmWaveForm();
  void compute(Sigui8 &O_data,Sigui8 &I_data);
  
  int get_input_sz(){
    return Nmax*2;
  }
  int get_output_sz(){
    return Ls*8;
  }

  int64 symCounter; //just for monitoring
  
private:
  int Lu     ;
  int Ls     ;
  int Ku     ;
  int Nmax   ;
  int NbTps  ;
 
  /*state counter*/
  int isym;//symbol number%68
  int ifrm;//frame number%4
  int iromtps;//%4624
  /*constant signals*/
  Sigui8 mask_cp;
  Sigui8 mask_tps; 
  Sigcxs tps_initmap;
  Sigcxs cx_prbs;
  /*state signals*/
  Sigcxs sigst_tpsmap;
  Sigcxs SCarriers; 
  Sigcxs SFreq;
  //Sigcxs STime;
  /*constant float*/
  float normFact;
#ifdef check_ofdm 
  FILE * DUMP_ofdm;
#endif
};

#endif
