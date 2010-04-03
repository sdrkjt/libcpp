#include "dvbgen_prbs.hh"

//#define debug


//----------------------------------------------------------//
//generate prbs sequence for Scrambling&Energy Dispersal    //
//1504bytes                                                 //
//----------------------------------------------------------//

void dvb_gen_scrmb_1504(Sigui8 &O_Scrmb)
{
  CHECK_INIT_SIG(O_Scrmb,1504);
  O_Scrmb = 0;
  short int state = 0xA9;

#ifdef debug 
  DUMPBIT(short int,state);
#endif

  unsigned int O_bit = 0;
  for(int ibyte=1;ibyte<1504;ibyte++)
    {
      for(int ibit=7;ibit>=0;ibit--)
	{
	  O_bit              = EXTRACTBIT(state,14) ^ EXTRACTBIT(state,15);
	  O_Scrmb[ibyte]     = ((O_Scrmb[ibyte]) | (O_bit<<ibit));
	  state              = (state<<1) | O_bit;

#ifdef debug
	  dump(O_bit);
	  DUMPBIT(short int,state);
	  if(ibit==0){
	    dump(ibyte);
	    DUMPBIT(char,O_Scrmb[ibyte]);
	    dump(O_Scrmb[ibyte]);
	  }
	  mygetchar();
#endif

	}
    }
  /**bypass SYNC bytes**/
  for(int isync = 0;isync<1504;isync+=188)
    O_Scrmb[isync] = 0;

}
