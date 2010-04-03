#ifndef dvbgen_outer_coder_hh
#define dvbgen_outer_coder_hh

#include "libsig.hh"
#include "dvbgen_prbs.hh"
#include "dvbgen_rs.hh"
#include "dvbgen_byteintrlv.hh"

//#define check_outer_coder

class OuterCoder {
public:
  enum{INPUT_SZ = 1504, OUTPUT_SZ = 1632};

#ifdef check_outer_coder 
  FILE * DUMP_scramble;
  FILE * DUMP_rsencode;
  FILE * DUMP_rsintrlv;
  FILE * DUMP_ts ;
#endif

  OuterCoder() 
  {
    /*----Scrambler Init--------------*/  
    O_Scrmb.SetSize(1504);
    //TIME();
    dvb_gen_scrmb_1504(O_Scrmb);
    //TIME("dvb_gen_scrmb_1503");
#ifdef check_outer_coder 
    DUMP_ts       = fopen ("z_dump/ts.bin", "wb");
    DUMP_scramble = fopen ("z_dump/scramble.bin", "wb");
    DUMP_rsencode = fopen ("z_dump/rsencode.bin", "wb");
    DUMP_rsintrlv = fopen ("z_dump/outer_intrlv.bin", "wb");
#endif
  }
  ~OuterCoder(){
#ifdef check_outer_coder
    fclose(DUMP_ts )     ; MSG("Write File: z_dump/ts.bin");
    fclose(DUMP_scramble); MSG("Write File: z_dump/scramble.bin");
    fclose(DUMP_rsencode); MSG("Write File: z_dump/rsencode.bin");
    fclose(DUMP_rsintrlv); MSG("Write File: z_dump/outer_intrlv.bin");
#endif
  }
  int get_input_sz(){
    return INPUT_SZ;
  }
  int get_output_sz(){
    return OUTPUT_SZ;
  }

  void compute(Sigui8 &O_rs8pck, Sigui8 &I_mpeg8pck){
#ifdef BOUNDSCHECKING
    if(I_mpeg8pck.size()!=INPUT_SZ )MSG_ERROR("Input Size is not correct"); /* TS output */
    if(O_rs8pck  .size()!=OUTPUT_SZ)MSG_ERROR("Output Size is not correct"); /* RS_INTRLV output */
#endif

#ifdef check_outer_coder
    fwrite(I_mpeg8pck.bufferA , 1 ,  I_mpeg8pck.size(), DUMP_ts );
#endif
    /****Scrambler*************************/
    //-bit-wise inverted the first SYNC in mpeg8pck -//
    I_mpeg8pck[0]= ~0x47;
    //TIME();
    I_mpeg8pck = I_mpeg8pck ^ O_Scrmb;
    //TIME("Scrambler 8pcks");                                /* 2.700Cycles/mpeg8pck */
#ifdef check_outer_coder
    fwrite(I_mpeg8pck.bufferA , 1 ,  I_mpeg8pck.size(), DUMP_scramble );
#endif
    /****RS Encoding + Byte Interleaving***/
    //TIME();
    LOOP(ipck,8){
      int rs_ptr = ipck*204; int mpeg_ptr = ipck*188;
      Sigui8 tmp1pck =  O_rs8pck(rs_ptr,rs_ptr+203); //reference
      tmp1pck(0,187) =  I_mpeg8pck(mpeg_ptr,mpeg_ptr+187);

      M_rsencoder  .encode1pck(tmp1pck);
#ifdef check_outer_coder
      fwrite(tmp1pck.bufferA , 1 ,  tmp1pck.size(), DUMP_rsencode );
#endif
      //M_byte_intrlv.do_intrlv_1pck(O_rs8pck(rs_ptr,rs_ptr+203), tmp1pck );
    }

    M_byte_intrlv.do_intrlv_8pck(O_rs8pck,O_rs8pck);

#ifdef check_outer_coder
    fwrite(O_rs8pck.bufferA , 1 , O_rs8pck.size(), DUMP_rsintrlv );
#endif
    //TIME("(RSencoder + Byte Interleaving) 8pcks");         /* 51.000(45.000+15.000)cycles/mpeg8pck */
    /* 45.000 without getchar               */
  };

private:
  /*----Scrambler Init--------------*/  
  Sigui8 O_Scrmb           ; 

  /*----RS+ByteInterleaver Init-----*/
  RS256 M_rsencoder;

  /*----Byte Interleaver Init-------*/
  ByteIntrlv M_byte_intrlv;
 
};

#endif
