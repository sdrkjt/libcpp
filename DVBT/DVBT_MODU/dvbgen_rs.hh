#ifndef dvbgen_rs_hh
#define dvbgen_rs_hh

#include "libsig.hh"

#define DEBUG_RS

typedef unsigned char gfPol ; /* polynomial representation */
typedef unsigned char gfPow ; /* power of primitif element */   


class GF256{
public:
  int pow2pol[256]; /* pow2pol[pow] = a^pow; convention: 255 is the power of element 0 */
  int pol2pow[256];
  int primPol     ; /* primitif polynomial  x8 + x4 + x3 + x2 + 1 <=> 0x11D */

  GF256();

};

class RS256{
public:
  RS256();

  /* inline method with return for speed */

  /* full version with 239bytes info */
  __v16qi encode239(const Sigui8 &I_239_InfoBytes);

  /* shortened version                                    */
  /* first 16bytes are parity, the next 188bytes are info */
  void encode1pck(const Sigui8 &IO_204Bytes);

private:
  GF256    gf          ;
  gfPow    genpoly[17] ;/* (x+1)*(x+a^1)*...*(x+a^15)), 17bytes */
  Sigv16qi feedback    ;/*  size = 256 x 16bytes = 4096bytes    */
  Sigui8   reg         ;/*  16bytes register                    */

};


#endif

