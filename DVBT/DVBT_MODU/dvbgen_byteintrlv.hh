#ifndef dvbgen_byteintrlv_hh
#define dvbgen_byteintrlv_hh


class ByteIntrlv {
public:

  ByteIntrlv(){
    inputSz = 1632; //8pcks
    memSz   = 3876;//(11+8)*17*12;
    Mem.zeros(memSz);

    for(uint16 ipck=0;ipck<8;ipck++){
      for(uint16 ii=0;ii<12;ii++){
	for(uint16 mm=0;mm<17;mm++){
	  uint16 inputIndx    = ipck*204+       mm*12+ii;
	  uint16 outputIndx   = ipck*204+ii*204+mm*12+ii; //7*204+11*204+16*12+11 = 3875
	  indxConv[inputIndx] = outputIndx;
	}
      }
    }

  }
  
  void do_intrlv_8pck(Sigui8 &O_intrlv1pck, const Sigui8 &I_intrlv1pck)
  {
    //TIME();
    for(uint16 idt=0;idt<inputSz;idt++){
      Mem[indxConv[idt]] = I_intrlv1pck[idt];
    }
    O_intrlv1pck               =   Mem(0,inputSz-1);
    Mem(0,memSz-inputSz-1)     =   Mem(inputSz,memSz-1);
    //Mem(memSz-inputSz,memSz-1) = 0;
    memset(Mem.bufferA+memSz-inputSz, 0, inputSz);
    //TIME("BYTE INTRLV 3");
}

private:
  uint16 indxConv[1632]; /* for index conversion */
  int memSz;
  int inputSz;

  Sigui8 Mem;
};

//#define use_lookuptable
//
///*use_lookuptable         : 15000cycles/1632bytes*
// *without use_lookuptable : 25000cycles/1632bytes*/

// class ByteIntrlv {
// public:

//   ByteIntrlv(){
// #ifdef use_lookuptable
//     for(uint8 ii=0;ii<204;ii++)
//       {
// 	uint8 irow = ii/12;
// 	uint8 icol = ii-12*irow;
// 	icnv[ii]  = 17*icol+irow;
//       }
// #endif
//   }
  
//   void do_intrlv_1pck(const Sigui8 &O_intrlv1pck, const Sigui8 &I_intrlv1pck)
//   {
//     //CHECK_INIT_SIG(O_intrlv1pck,204);

//     for(uint8 ii=0;ii<204;ii++)
//       {
// #ifdef use_lookuptable
// 	{
// 	  O_intrlv1pck[icnv[ii]] =  I_intrlv1pck[ii];
// 	}
// #else
// 	{
// 	  uint8 irow = ii/12;
// 	  uint8 icol = ii-12*irow;
// 	  O_intrlv1pck[17*icol+irow] =  I_intrlv1pck[ii];
// 	}
// #endif
//       }
//   }

// private:
//   uint8 icnv[204]; /* for index conversion */

// };


#endif
