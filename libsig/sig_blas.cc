#include "sig_blas.hh"
#include <emmintrin.h>
#include <math.h>

float   cpp_dotproduct_fff(const Sigfl &x,const Sigfl &y)
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
    

float  sse_dotproduct_fff(const Sigfl &xv, const Sigfl  &yv)
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

Cxs cpp_dotproduct_ccf(const Sigcxs &xv, const Sigfl  &yv)
{
  float sum0 = 0;
  float sum1 = 0;
  
  int sz = xv.size();
  float * adrx = (float*)xv.getptr();
  float * adry =  yv.getptr();
 
  for(int ii=0;ii<sz;ii++){
    sum0 += adry[0] * adrx[0];
    sum1 += adry[0] * adrx[1];
    adry += 1;
    adrx += 2;
  };
  
  return Cxs(sum0,sum1);
}

Cxs  sse_dotproduct_ccf(const Sigcxs &xv, const Sigfl  &yv)
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

  float  *adrx = (float*)xv.getptr();
  float  *adry = yv.getptr(); 

  __m128 tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8;
  __m128 accu1,accu2,accu3,accu4;

  accu1  = _mm_xor_ps(accu1 ,accu1); 
  accu2  = _mm_xor_ps(accu2 ,accu2);
  accu3  = _mm_xor_ps(accu3 ,accu3);
  accu4  = _mm_xor_ps(accu4 ,accu4);
  /*unloop x4 is better than x8
    why?: the raison is that the cache line size is 64bytes = 4 * 4floats
   */
  for(int ii=0;ii<nb16;ii+=8){
    tmp5 = *((__m128*)(adrx)); tmp6 = *((__m128*)(adrx+4));
    tmp7 = _mm_shuffle_ps(tmp5,tmp6,_MM_SHUFFLE(2,0,2,0)); //real part
    tmp8 = _mm_shuffle_ps(tmp5,tmp6,_MM_SHUFFLE(3,1,3,1)); //image part

    tmp1  =  _mm_mul_ps(tmp7, *((__m128*)(adry)) ) ; 
    accu1 =  _mm_add_ps(tmp1, accu1);

    tmp2  =  _mm_mul_ps(tmp8, *((__m128*)(adry)) ) ; 
    accu2 =  _mm_add_ps(tmp2, accu2);

    tmp5 = *((__m128*)(adrx+8)); tmp6 = *((__m128*)(adrx+12));
    tmp7 = _mm_shuffle_ps(tmp5,tmp6,_MM_SHUFFLE(2,0,2,0));
    tmp8 = _mm_shuffle_ps(tmp5,tmp6,_MM_SHUFFLE(3,1,3,1));

    tmp1  =  _mm_mul_ps(tmp7, *((__m128*)(adry+4)) ) ; 
    accu3 =  _mm_add_ps(tmp1, accu3);

    tmp2  =  _mm_mul_ps(tmp8, *((__m128*)(adry+4)) ) ; 
    accu4 =  _mm_add_ps(tmp2, accu4);

    adrx += 16;
    adry += 8;
  }
  for(int jj=nb16;jj<nb16+nb4;jj+=4){
    tmp5 = *((__m128*)(adrx)); tmp6 = *((__m128*)(adrx+4));
    tmp7 = _mm_shuffle_ps(tmp5,tmp6,_MM_SHUFFLE(2,0,2,0));
    tmp8 = _mm_shuffle_ps(tmp5,tmp6,_MM_SHUFFLE(3,1,3,1));

    tmp1  =  _mm_mul_ps(tmp7, *((__m128*)(adry)) ) ; 
    accu1 =  _mm_add_ps(tmp1, accu1);

    tmp2  =  _mm_mul_ps(tmp8, *((__m128*)(adry)) ) ; 
    accu2 =  _mm_add_ps(tmp2, accu2);

    adrx += 8;
    adry += 4;
  }

  accu1 = _mm_add_ps(accu1,accu3);//B1 B2 B3 B4
  accu2 = _mm_add_ps(accu2,accu4);//A1 A2 A3 A4

  /*TODO: optimization the following part of the code*/
  /*real part*/
  float res_re;
  __m128 tmp10;  //_mm_xorps(tmp0,tmp0);
  tmp10 = _mm_movehl_ps(tmp10,accu1);	// tmp0 = 0 0 d1 d2
  tmp10 = _mm_add_ps(tmp10,accu1);         // tmp0 = d1 d2 d1+d3 d2+d4
  __m128 tmp10_shuffle;
  tmp10_shuffle = _mm_shuffle_ps(tmp10,tmp10,_MM_SHUFFLE(3,2,0,1)); //tmp0_shuffle =   d1 d2 d2+d4 d1+d3
  tmp10 = _mm_add_ss(tmp10,tmp10_shuffle);
  _mm_store_ss(&res_re,tmp10);
  /*image part*/
  float res_im;
  __m128 tmp11;  //_mm_xorps(tmp11,tmp11);
  tmp11 = _mm_movehl_ps(tmp11,accu2);	// tmp11 = 0 0 d1 d2
  tmp11 = _mm_add_ps(tmp11,accu2);      // tmp11 = d1 d2 d1+d3 d2+d4
  __m128 tmp11_shuffle;
  tmp11_shuffle = _mm_shuffle_ps(tmp11,tmp11,_MM_SHUFFLE(3,2,0,1)); //tmp11_shuffle =   d1 d2 d2+d4 d1+d3
  tmp11 = _mm_add_ss(tmp11,tmp11_shuffle);
  _mm_store_ss(&res_im,tmp11);

  for(int kk=0;kk<nbRemain4;kk++){
    res_re += *(   adrx   ) * *(adry);
    res_im += *(1+(adrx++)) * *(adry++);
  }


  return Cxs(res_re,res_im);
}

/***************Start***functions that use dotproduct******************/
float meanAbs2(const Sigfl  &x){
  int sz = x.size();
  return sse_dotproduct_fff(x,x)/sz;
};

float meanAbs2(const Sigcxs &x){
  int sz = x.size();
  Sigfl x2fl = Sigfl(x);
  return sse_dotproduct_fff(x2fl,x2fl)/sz;
};

// float meanAbs2(const Sigcxd &x){
//   int sz = x.size();
//   Sigdb x2db = Sigdb(x);
//   return sse_dotproduct_ddd(x2db,x2db)/sz;
// };

/***************End*****functions that use dotproduct******************/
