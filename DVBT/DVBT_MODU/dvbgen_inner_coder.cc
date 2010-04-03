#include "dvbgen_inner_coder.hh"

void InnerCoder::gen_tab()
{
  int NDAT = 0, NBIT = 0;
  switch (crate){
  case DvbParam::CR12:
    {
      NDAT = 256;
      NBIT = 8;
	
    }break;
  case DvbParam::CR23:
    {
      NDAT = 256;
      NBIT = 8;
    }break;
  case DvbParam::CR34:
    {
      NDAT = 64;
      NBIT = 6;
    }break;
  case DvbParam::CR56:
    {
      NDAT = 32;
      NBIT = 5;
    }break;
  case DvbParam::CR78:
    {
      NDAT = 128;
      NBIT = 7;
    }break;
  default:
    {
      MSG_ERROR("Code Rate Undefined!!abort");
    }
    break;
  }

#ifndef TEST_ARRAY
  lktab_X = memalloc<uint8>(256,64);
  lktab_Y = memalloc<uint8>(256,64);
#endif

  lktab   = memalloc<uint16>(NDAT,64);
  
  /* gen lookup table */
  for(int iSt=0;iSt<64;iSt++) /* loop on state */
    {

      for(int iDt=0; iDt<NDAT; iDt++ ) /* loop on data */
	{
	  uint8 curSt=0; /* only 7 bits are used */
	  for(int ii=1;ii<7;ii++){ //inverse order      
	    SETBIT(curSt,7-ii,GETBIT(iSt,ii));
	  }

	  uint8  byte_X = 0;
	  uint8  byte_Y = 0;
	  uint16 punc12 = 0; /* X1Y1 X2Y2 X3Y3 X4Y4 X5Y5 X6Y6 X7Y7 X8Y8 */
	  uint16 punc23 = 0; /* X1Y1Y2    X3Y3Y4    X5Y5Y6    X7Y7Y8    */
	  uint8  punc34 = 0; /* X1Y1Y2X3       X4Y4Y5X6                 */
	  uint8  punc56 = 0;
	  uint8  punc78 = 0;

#ifdef DEBUG_INNER
	  DUMPBIT(char,iDt);
	  DUMPBIT(char,iSt);
#endif

	  /* MSB bit first */
	  for(int iBit=NBIT;iBit>0;iBit--){
	    int curbit = GETBIT(iDt,iBit);
	
	    SETBIT(curSt,7,curbit);
	    int bit_X = bitc(G1 & curSt);
	    int bit_Y = bitc(G2 & curSt);

#ifdef DEBUG_INNER
	    DUMP(curbit);	
	    DUMPBIT(char, G1);
	    DUMPBIT(char, G2);
	    DUMPBIT(char,curSt);
	    DUMP(bit_X);
	    DUMP(bit_Y);
#endif

	    /* MSB bit in, MSB bit out */
	    SETBIT(byte_X,iBit,bit_X);
	    SETBIT(byte_Y,iBit,bit_Y);
	    /*C12 output*/
	    SETBIT(punc12,2*iBit  ,bit_X);
	    SETBIT(punc12,2*iBit-1,bit_Y);
	    /*C23 output*/
	    if((iBit&0x1)==0){
	      SETBIT(punc23,3* iBit   /2  ,bit_X); //X1
	      SETBIT(punc23,3* iBit   /2-1,bit_Y); //Y1
	    }
	    else{
	      SETBIT(punc23,3*(iBit+1)/2-2,bit_Y); //Y2
	    }
	    /*C34 output*/
	    /*C56 output*/
	    /*C78 output*/


	    curSt = curSt>>1;

#ifdef DEBUG_INNER
	    DUMPBIT(char,byte_X);
	    DUMPBIT(char,byte_Y);
#endif

	  }

	  lktab_X[iDt][iSt] = byte_X;
	  lktab_Y[iDt][iSt] = byte_Y;
	  
	  uint16 punc12inv = 0;//byte inverted
	  uint8* pinvt = (uint8*)(&punc12inv);
	  uint8* porig = (uint8*)(&punc12   );
	  pinvt[0] = porig[1];pinvt[1] = porig[0];//byte0 commes first then byte1

	  lktab[iDt][iSt] = (crate==DvbParam::CR12)*punc12inv + 
	                    (crate==DvbParam::CR23)*punc23    +  
	                    (crate==DvbParam::CR34)*punc34    + 
	                    (crate==DvbParam::CR56)*punc56    +
	                    (crate==DvbParam::CR78)*punc78;
	}
	
#ifdef DEBUG_INNER
      getchar();
#endif
 
    }

//   for(int ii=0;ii<256;ii++){
//     cout<<"**************"<<ii<<"************\n";
//     for(int jj=0;jj<64;jj++)
//       cout<<(int)lktab_Y[ii][jj]<<" ";
//   }
//   mygetchar();
}
