#include "sig_blas.hh"
#include <emmintrin.h>
#include <math.h>

float   sdot_cpp(const Sigfl &x,const Sigfl &y)
{ 
  float *adrx = x.getptr();
  float *adry = y.getptr(); 
  int       n = x.size();

  float  s=0.0;    
  while (n--)     { s+=*(adrx++) * *(adry++); };
  return s;
}

#define  motif_sdot(ii,tmp,accu) tmp =   _mm_mul_ps( *((__m128*)(adrx+ii)), *((__m128*)(adry+ii)) ) ; \
    accu =  _mm_add_ps(tmp, accu);					\
    
float  sdot_sse(const Sigfl &xv, const Sigfl  &yv)
{ 
  int nv = xv.size();
  int nbRemain16 = nv&15       ; int nb16 = nv         - nbRemain16 ;			
  int nbRemain4  = nbRemain16&3; int nb4  = nbRemain16 - nbRemain4  ;  

#ifdef BOUNDSCHECKING
  int size_y = yv.size();			
  if(size_y!=nv)				
    {								
      cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	
      myabort();						
    };								
#endif

  float  *adrx = xv.getptr();
  float  *adry = yv.getptr(); 

  __m128 tmp1,tmp2,tmp3,tmp4,tmp5;
  __m128 accu1,accu2,accu3,accu4;

  accu1  = _mm_xor_ps(accu1 ,accu1); 
  accu2  = _mm_xor_ps(accu2 ,accu2);
  accu3  = _mm_xor_ps(accu3 ,accu3);
  accu4  = _mm_xor_ps(accu4 ,accu4);
  /*unloop x4 is better than x8
    why?: the raison is that the cache line size is 64bytes = 4 * 4floats
   */
  for(int ii=0;ii<nb16;ii+=16){
    motif_sdot(ii   ,tmp1,accu1);
    motif_sdot(ii+4 ,tmp2,accu2);
    motif_sdot(ii+8 ,tmp3,accu3);
    motif_sdot(ii+12,tmp4,accu4);
  }
  for(int jj=nb16;jj<nb16+nb4;jj+=4){
    motif_sdot(jj   ,tmp5,accu1);
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

  adrx += nv - nbRemain4;
  adry += nv - nbRemain4;
  for(int kk=0;kk<nbRemain4;kk++){
    res += *(adrx++) * *(adry++);
  }
  return res;
}

float meanAbs2(const Sigfl  &x){
  int sz = x.size();
  return sdot_sse(x,x)/sz;
};

float meanAbs2(const Sigcxs &x){
  int sz = x.size();
  Sigfl x2fl = Sigfl(x);
  return sdot_sse(x2fl,x2fl)/sz;
};

// float meanAbs2(const Sigcxd &x){
//   int sz = x.size();
//   Sigdb x2db = Sigdb(x);
//   return sdot_sse(x2db,x2db)/sz;
// };

