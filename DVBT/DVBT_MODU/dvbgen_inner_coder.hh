#ifndef dvbgen_innercoder_hh
#define dvbgen_innercoder_hh

#include "libsig.hh"
#include "dvbgen_param.hh"

/* bit counter, even/odd = 1/0  */
static inline int bitc(const uint8& Ibyte){
  int checksum = 0;
  for(int ibit=0;ibit<8;ibit++)
    checksum += GETBIT(Ibyte,ibit);
  return checksum & 0x1;
}

//#define DEBUG_INNER

//#define TEST_ARRAY // 12.000Cycles if array is used
                   // 18.000Cycles if dyn alloc is used

//#define check_inner_coder

class InnerCoder {
public:
  int nbByteIn;
  int nbByteOut;

#ifdef check_inner_coder 
  FILE * DUMP_puncturing;
  FILE * DUMP_convol;
  FILE * DUMP_input;
#endif

  void gen_tab();

  InnerCoder(const int &imode)
  {
    G1    = 0171;
    G2    = 0133;
    state = 0;
    crate = imode;

    nbByteIn  = 1260;
    nbByteOut = nbByteIn*(crate+1)/crate;

    gen_tab();

#ifdef check_inner_coder 
    DUMP_puncturing      = fopen ("z_dump/puncturing.bin", "wb");
    DUMP_convol          = fopen ("z_dump/convol_encode.bin", "wb");
    DUMP_input           = fopen ("z_dump/inner_coder_input.bin", "wb");
#endif

  }
  ~InnerCoder(){
#ifndef TEST_ARRAY
    memfree(lktab_X);
    memfree(lktab_Y);
#endif
    memfree(lktab  );

#ifdef check_inner_coder 
    fclose(DUMP_puncturing )     ; MSG("Write File: z_dump/puncturing.bin");
    fclose(DUMP_convol )         ; MSG("Write File: z_dump/convol_encode.bin");
    fclose(DUMP_input )          ; MSG("Write File: z_dump/inner_coder_input.bin");
#endif
  }

  /* inline  encode1byte */
  void encode1byte(uint8 &O_X,uint8 &O_Y, const uint8 &I_byte)
  {
    O_X   =  *(lktab_X[0]+(I_byte<<6)+state);//*(*(lktab_X+I_byte)+state);//lktab_X[ I_byte ][ state ];//
    O_Y   =  *(lktab_Y[0]+(I_byte<<6)+state);//*(*(lktab_Y+I_byte)+state);//lktab_Y[ I_byte ][ state ];

    state = I_byte & 0x3F;
  };

  void encode_punture_C12(uint16 &O_XY, const uint8 &I_byte)
  {
    O_XY  = lktab[I_byte][state];
    state = I_byte & 0x3F;
  };
  void encode_punture_1260_C12(const Sigui8 &O_vecbyte, const Sigui8 &I_vecbyte)
  {
#ifdef BOUNDSCHECKING
    if(O_vecbyte.size()!=nbByteOut || I_vecbyte.size()!=nbByteIn){
      DUMP(nbByteOut);DUMP(nbByteIn);
      myabort();
    }
#endif
    Sigui16 O_vec2byte = Sigui16(O_vecbyte);
    for(int ibyte=0;ibyte<nbByteIn;ibyte++)
      {
	//O_vecbyte must be read from low index to hight index
	encode_punture_C12(O_vec2byte[ibyte],I_vecbyte[ibyte]);
      }
  };

  void encode_punture_C23(uint24 &O_XY, const uint8 &I_byte1,const uint8 &I_byte2)
  {
    uint16 XY1  = lktab[I_byte1][state];

#ifdef check_inner_coder
    fwrite(&(lktab_X[I_byte1][state]) , 1 , 1 , DUMP_convol );
    fwrite(&(lktab_Y[I_byte1][state]) , 1 , 1 , DUMP_convol );
#endif

    state = I_byte1 & 0x3F; 

    uint16 XY2  = lktab[I_byte2][state];

#ifdef check_inner_coder
    fwrite(&(lktab_X[I_byte2][state]) , 1 , 1 , DUMP_convol );
    fwrite(&(lktab_Y[I_byte2][state]) , 1 , 1 , DUMP_convol );
#endif

    state = I_byte2 & 0x3F;

    // uint32* XY = (uint32*)&O_XY;
    // *XY = ((*XY) & 0xff000000) + (XY1<<12) + XY2;//thread unsafe
    uint32 XY   = (XY1<<12) + XY2;
    uint8* pXY  = (uint8*)(&XY);
    uint8* pOut = (uint8*)(&O_XY);
    pOut[0] = pXY[2]; pOut[1] = pXY[1];pOut[2] = pXY[0];//pOut[0] is send first then pOut[1], pOut[2]
  };

  void encode_punture_1260_C23(const Sigui8 &O_vecbyte, const Sigui8 &I_vecbyte)
  {
#ifdef BOUNDSCHECKING
    if(O_vecbyte.size()!=nbByteOut || I_vecbyte.size()!=nbByteIn){
      DUMP(nbByteOut);DUMP(nbByteIn);
      myabort();
    }


#endif
    Sigui24 O_vec_ui24 = Sigui24(O_vecbyte);
    int half_nbByteIn = nbByteIn>>1;
    for(int ibyte=0;ibyte<half_nbByteIn;ibyte++)
      {
	//O_vecbyte must be read from hight index to low index	
	//encode_punture_C23(O_vec_ui24(half_nbByteIn-ibyte-1),I_vecbyte(ibyte<<1),I_vecbyte((ibyte<<1)+1));	

	//O_vecbyte must be read from low index to hight index	
	encode_punture_C23(O_vec_ui24[ibyte],I_vecbyte[ibyte<<1],I_vecbyte[(ibyte<<1)+1]);
      }
  };

  void compute(Sigui8  &O_data,  Sigui8  &I_data){
#ifdef check_inner_coder
      fwrite(I_data.bufferA, 1 , nbByteIn , DUMP_input );
#endif
 
    //TIME();
    if(crate==DvbParam::CR12)encode_punture_1260_C12(O_data,I_data);
    if(crate==DvbParam::CR23)encode_punture_1260_C23(O_data,I_data);
    //TIME("Convolutional Encoder 1260Bytes"); 

#ifdef check_inner_coder
    //for(int ii=nbByteOut-1;ii>-1;ii--){
    for(int ii=0;ii<nbByteOut;ii++){
      fwrite(O_data.bufferA+ii , 1 , 1 , DUMP_puncturing );
    }
#endif
  }

private:

  uint8 G1,G2; /* generator polynomials */
  uint8 crate;
  uint8 state;
#ifndef TEST_ARRAY
  uint8** lktab_X ;//[256][64]; X output
  uint8** lktab_Y ;//[256][64]; Y output
#else
  uint8 lktab_X[256][64];
  uint8 lktab_Y[256][64];
#endif
  uint16 ** lktab ;// XY punctured output 
  //  uint16 lktab12[256][64];/*8bits input,16bits output*/
  //  uint16 lktab23[256][64];/*8bits input,12bits output*/
  //  uint8  lktab34[64 ][64];/*6bits input,8 bits output*/
  //  uint8  lktab56[32 ][64];/*5bits input,6 bits output*/
  //  uint8  lktab78[128][64];/*7bits input,8 bits output*/ 
 

};


#endif
