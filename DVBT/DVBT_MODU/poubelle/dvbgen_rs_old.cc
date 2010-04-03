#include "dvbgen_rs.hh"

static inline void check_gf256(int* pol2pow);
static inline void check_rs256(gfPol* genpoly);

GF256::GF256()
{
  primPol = 0x11D;

  /* compute pow2pol tab */
  pow2pol[0] = 0x1;
  for(int ii=1 ; ii<255 ; ii++)
    {
      /* x^ii mod P(x) = ( x * (x^(ii-1) mod P(x)) ) mod P(x) */
      pow2pol[ii] = pow2pol[ii-1]<<1;
      if ( EXTRACTBIT(pow2pol[ii],9) )
	pow2pol[ii] ^= primPol;
    }
  /* 0 at the end */
  pow2pol[255] = 0x0; 
    
  /* compute pol2pow tab */
  for(int ii=0 ; ii<256 ; ii++)
    pol2pow[pow2pol[ii]] = ii;

#ifdef DEBUG_RS
  check_gf256(pol2pow);
#endif

}

static inline gfPow gf256_mulpow(const gfPow &a,const gfPow &b)
{
  gfPow res; 
  res = (a + b) % 255;
  if((a==255)||(b==255))
    res = 255;

  return res;
}

RS256::RS256()
{
  /* compute  polynomial generator */
  //representation by pol
    
  for (int ii=0 ; ii<17 ; ii++)       genpoly[ii] = 0;
    
  genpoly[0] = 0x1;
  genpoly[1] = 0x1;
    
  for(int ii=1;ii<16;ii++) /* mul with (x+a^ii) */
    {      
      gfPol oldval = 0;
      for (int jj=0 ; jj<17 ; jj++)
	{
	  gfPow tmp; gfPol newval;
	  tmp         = gf256_mulpow(ii , gf.pol2pow[genpoly[jj]]); /* (genpoly[jj]*a^ii) */
	  newval      = gf.pow2pol[tmp] ^ oldval;
	  oldval      = genpoly[jj];
	  genpoly[jj] = newval   ;
	}
    }

#ifdef DEBUG_RS
  check_rs256(genpoly);
#endif

  //representation by pow
  for(int ii=0;ii<17;ii++)
    genpoly[ii] = gf.pol2pow[genpoly[ii]];

  /* compute feedback             */
  Siguch  feedback_byte;
  feedback_byte.SetSize(4096);

  LOOP(ii,256){ //datain = ii (pol representation)
    LOOP(jj,16){
      feedback_byte(ii*16+jj) = gf.pow2pol[gf256_mulpow(genpoly[jj],gf.pol2pow[ii])]; 
    }
  }
  feedback = Sigv16qi(feedback_byte);
  /* alloc reg */
  reg.SetSize(16);
}

__v16qi RS256::encode239(const Siguch &I_239_InfoBytes)
{
  __v16qi* mm_reg = (__v16qi*)reg.bufferA;
   
  memset( (char*)mm_reg,0,16);

  for(int ii=238;ii>=0;ii--){
    gfPol dataIn;  gfPol dataFeedBack; __v16qi mm_feedback;

    dataIn         = I_239_InfoBytes(ii);
    dataFeedBack   = dataIn ^ reg.val(15); /* because genpoly[16] = 1 */
    mm_feedback    = feedback.val(dataFeedBack); 

    /* left shift */
    *mm_reg = _mm_slli_si128( *mm_reg, 1);
    /* adding feedback */
    *mm_reg = (*mm_reg) ^ mm_feedback;  
  }

  return *mm_reg;
}

void RS256::encode1pck(const Siguch &IO_204Bytes)
{
  /* use register to aligne data   */
  /* input is not necessary aligned*/
  __v16qi* mm_reg = (__v16qi*)reg.bufferA;
   
  memset( (char*)mm_reg,0,16);

  for(int ii=203;ii>15;ii--){
    gfPol dataIn;  gfPol dataFeedBack; __v16qi mm_feedback;

    dataIn         = IO_204Bytes(ii);
    dataFeedBack   = dataIn ^ reg(15);/* because genpoly[16] = 1 */
    mm_feedback    = feedback(dataFeedBack); 

    /* left shift */
    *mm_reg = _mm_slli_si128(*mm_reg, 1);
    /* adding feedback */
    *mm_reg = (*mm_reg) ^ mm_feedback;  //_mm_xor_si128(*mm_reg, mm_feedback);//
  }

  memcpy(IO_204Bytes.bufferA,(char*)mm_reg,16);
}



/********************************************************************************************/
short _gf_256_log[256] = {
255 , 0x00, 0x01, 0x19, 0x02, 0x32, 0x1a, 0xc6, 0x03, 0xdf, 0x33, 0xee, 0x1b, 0x68, 0xc7, 0x4b,
0x04, 0x64, 0xe0, 0x0e, 0x34, 0x8d, 0xef, 0x81, 0x1c, 0xc1, 0x69, 0xf8, 0xc8, 0x08, 0x4c, 0x71,
0x05, 0x8a, 0x65, 0x2f, 0xe1, 0x24, 0x0f, 0x21, 0x35, 0x93, 0x8e, 0xda, 0xf0, 0x12, 0x82, 0x45,
0x1d, 0xb5, 0xc2, 0x7d, 0x6a, 0x27, 0xf9, 0xb9, 0xc9, 0x9a, 0x09, 0x78, 0x4d, 0xe4, 0x72, 0xa6,
0x06, 0xbf, 0x8b, 0x62, 0x66, 0xdd, 0x30, 0xfd, 0xe2, 0x98, 0x25, 0xb3, 0x10, 0x91, 0x22, 0x88,
0x36, 0xd0, 0x94, 0xce, 0x8f, 0x96, 0xdb, 0xbd, 0xf1, 0xd2, 0x13, 0x5c, 0x83, 0x38, 0x46, 0x40,
0x1e, 0x42, 0xb6, 0xa3, 0xc3, 0x48, 0x7e, 0x6e, 0x6b, 0x3a, 0x28, 0x54, 0xfa, 0x85, 0xba, 0x3d,
0xca, 0x5e, 0x9b, 0x9f, 0x0a, 0x15, 0x79, 0x2b, 0x4e, 0xd4, 0xe5, 0xac, 0x73, 0xf3, 0xa7, 0x57,
0x07, 0x70, 0xc0, 0xf7, 0x8c, 0x80, 0x63, 0x0d, 0x67, 0x4a, 0xde, 0xed, 0x31, 0xc5, 0xfe, 0x18,
0xe3, 0xa5, 0x99, 0x77, 0x26, 0xb8, 0xb4, 0x7c, 0x11, 0x44, 0x92, 0xd9, 0x23, 0x20, 0x89, 0x2e,
0x37, 0x3f, 0xd1, 0x5b, 0x95, 0xbc, 0xcf, 0xcd, 0x90, 0x87, 0x97, 0xb2, 0xdc, 0xfc, 0xbe, 0x61,
0xf2, 0x56, 0xd3, 0xab, 0x14, 0x2a, 0x5d, 0x9e, 0x84, 0x3c, 0x39, 0x53, 0x47, 0x6d, 0x41, 0xa2,
0x1f, 0x2d, 0x43, 0xd8, 0xb7, 0x7b, 0xa4, 0x76, 0xc4, 0x17, 0x49, 0xec, 0x7f, 0x0c, 0x6f, 0xf6,
0x6c, 0xa1, 0x3b, 0x52, 0x29, 0x9d, 0x55, 0xaa, 0xfb, 0x60, 0x86, 0xb1, 0xbb, 0xcc, 0x3e, 0x5a,
0xcb, 0x59, 0x5f, 0xb0, 0x9c, 0xa9, 0xa0, 0x51, 0x0b, 0xf5, 0x16, 0xeb, 0x7a, 0x75, 0x2c, 0xd7,
0x4f, 0xae, 0xd5, 0xe9, 0xe6, 0xe7, 0xad, 0xe8, 0x74, 0xd6, 0xf4, 0xea, 0xa8, 0x50, 0x58, 0xaf
};
short _rs_256_genpoly[17] = {
  //59,36,50,98,229,41,65,163,8,30,209,68,189,104,13,59,1
  0x3b, 0x24, 0x32, 0x62, 0xe5, 0x29, 0x41, 0xa3, 0x08, 0x1e, 0xd1, 0x44, 0xbd, 0x68, 0x0d, 0x3b, 0x01
};

static inline void check_gf256(int* pol2pow)
{
  cout<<"...check GF generation\n";
  LOOP(ii,256)
    if(pol2pow[ii]- _gf_256_log[ii] != 0)
      {
	DUMP(ii);
	cout<<"mismatch!!\n";
	myabort();
      }
  cout<<c_blue<<"GF generation: Passed\n"<<c_normal;
}

static inline void check_rs256(gfPol* genpoly)
{
  cout<<"...check RS polgen computation\n";
  LOOP(ii,17){
    //printf("0x%02x, ",_rs_256_genpoly[ii]);
    if(genpoly[ii]- _rs_256_genpoly[ii] != 0)
      {
	DUMP(ii);
	cout<<"mismatch!!\n";
	myabort();
      }
  }
  cout<<c_blue<<"RS polgen computation: Passed\n"<<c_normal;
}
/********************************************************************************************/
