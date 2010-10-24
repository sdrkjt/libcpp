#include "sig_blas.hh"
#include <emmintrin.h>
#include <math.h>

float   sdot_cpp(int n, float  *x,float  *y) { float  s=0.0;    while (n--)     { s+=*(x++) * *(y++); }; return s; }

#define  motif_sdot(ii,tmp,accu) tmp =   _mm_mul_ps( *((__m128*)(xv+ii)), *((__m128*)(yv+ii)) ) ; \
    accu =  _mm_add_ps(tmp, accu);					\

float  sdot_sse(int   nv, float  *xv, float  *yv)
{ 
  __m128 tmp1,tmp2,tmp3,tmp4;
  __m128 accu1,accu2,accu3,accu4;

  accu1  = _mm_xor_ps(accu1 ,accu1); 
  accu2  = _mm_xor_ps(accu2 ,accu2);
  accu3  = _mm_xor_ps(accu3 ,accu3);
  accu4  = _mm_xor_ps(accu4 ,accu4);

  for(int ii=0;ii<nv;ii+=16){
    motif_sdot(ii   ,tmp1,accu1);
    motif_sdot(ii+4 ,tmp2,accu2);
    motif_sdot(ii+8 ,tmp3,accu3);
    motif_sdot(ii+12,tmp4,accu4);
  }
  __m128 accu;
  accu2 = _mm_add_ps(accu2,accu1);
  accu4 = _mm_add_ps(accu4,accu3);
  accu  = _mm_add_ps(accu4,accu2);

  __m128 tmp0;  //_mm_xorps(tmp0,tmp0);
  tmp0 = _mm_movehl_ps(tmp0,accu);	// tmp0 = 0 0 d1 d2
  tmp0 = _mm_add_ps(tmp0,accu);         // tmp0 = d1 d2 d1+d3 d2+d4
  __m128 tmp0_shuffle;
  tmp0_shuffle = _mm_shuffle_ps(tmp0,tmp0,_MM_SHUFFLE(3,2,0,1)); //tmp0_shuffle =   d1 d2 d2+d4 d1+d3
  tmp0 = _mm_add_ss(tmp0,tmp0_shuffle);

  float res;
  _mm_store_ss(&res,tmp0);

  return res;
}
