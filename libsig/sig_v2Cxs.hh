#ifndef sig_v2Cxs_hh
#define sig_v2Cxs_hh

#include "cpp_monit.hh"
#include "sig_types.hh"
#include <emmintrin.h>
#ifdef SSE3
#include <pmmintrin.h>
#endif

#include "sig_complex.hh"
typedef Cx<float>   Cxs;

class __attribute__ (( aligned(16) ))  __v2Cxs{
protected:
  __m128 vec;
public:
  // Constructor to convert from type __m128 used in intrinsics:
  __v2Cxs(__m128 const & x) {vec = x;}
  __v2Cxs(Cxs const& x,Cxs const& y){
    vec = _mm_set_ps(x.Re(),x.Im(),y.Re(),y.Im());
  }
  operator __m128() const {return vec;}
};

// complex + complex: add both parts:
static inline __v2Cxs operator + (__v2Cxs const & a,
__v2Cxs const & b) {
  return (__v2Cxs)_mm_add_ps(a, b);}

// complex - complex: subtract both parts:
static inline __v2Cxs operator - (__v2Cxs const & a,
__v2Cxs const & b) {
  return (__v2Cxs)_mm_sub_ps(a, b);}

// complex * complex: x=(a,c) * y=(b,d) 3-2-1-0 (c.im c.re a.im a.re)
//(a.re*b.re-a.im*b.im, a.re*b.im+b.re*a.im).
//(c.re*d.re-c.im*d.im, c.re*d.im+d.re*c.im).
// This version is for SSE2 instruction set. It is more efficient
// to use _mm_addsub_pd and _mm_hadd_pd if SSE3 is available.
#ifdef SSE2
static inline __v2Cxs operator * (__v2Cxs const & x,
__v2Cxs const & y) {
   __m128 ac_flipped;                 // matrix=(a.im,a.re,c.im,c.re)  bits=(c.re c.im a.re a.im)
   __m128 bd_re;                      //        (b.re,b.re,d.re,d.re)       (d.re d.re b.re b.re)
   __m128 bd_im;                      //        (-b.im,b.im,-d.im,d.im)     (d.im -d.im b.im -b.im)
   static const union {               // (signbit,0)
      int i[4]; __m128 v;
   } signbitlow = {0x80000000,0,0x80000000,0};//0||0x80000000||0||0x80000000
   bd_im = _mm_shuffle_ps(y,y,_MM_SHUFFLE(3,3,1,1));      // Imag. part of b in both
  
   bd_re = _mm_shuffle_ps(y,y,_MM_SHUFFLE(2,2,0,0));      // Real part of b in  both
  
   ac_flipped = _mm_shuffle_ps(x,x,_MM_SHUFFLE(2,3,0,1)); // Swap real and imag parts of a
  
   bd_im = _mm_xor_ps(bd_im, signbitlow.v); // Change sign of low
      
   // Multiply and add:
   return (__v2Cxs)_mm_add_ps(_mm_mul_ps(x, bd_re) , _mm_mul_ps(ac_flipped, bd_im));
}
#else //!msse2
#ifdef SSE3
static inline __v2Cxs operator * (__v2Cxs const & x,
__v2Cxs const & y) {
   __m128 ac_flipped;                 // matrix=(a.im,a.re,c.im,c.re)  bits=(c.re c.im a.re a.im)
   __m128 bd_re;                      //        (b.re,b.re,d.re,d.re)       (d.re d.re b.re b.re)
   __m128 bd_im;                      //        (b.im,b.im,d.im,d.im)       (d.im d.im b.im b.im)
   bd_im = _mm_shuffle_ps(y,y,_MM_SHUFFLE(3,3,1,1));      // Imag. part of b in both
  
   bd_re = _mm_shuffle_ps(y,y,_MM_SHUFFLE(2,2,0,0));      // Real part of b in  both
  
   ac_flipped = _mm_shuffle_ps(x,x,_MM_SHUFFLE(2,3,0,1)); // Swap real and imag parts of a
      
   // Multiply and add:
   return (__v2Cxs)_mm_addsub_ps(_mm_mul_ps(x, bd_re) , _mm_mul_ps(ac_flipped, bd_im));
}
#else //!msse3
static inline __v2Cxs operator * (__v2Cxs const & x,
				  __v2Cxs const & y) 
{
  myabort(); 
  return __v2Cxs(Cxs(-1111,-1111),Cxs(-1111,-1111));
}
#endif //sse3
#endif //sse2

// complex / complex:
// (a.re*b.re+a.im*b.im, b.re*a.im-a.re*b.im)/(b.re*b.re+b.im*b.im).
// This version is for SSE2 instruction set. It is more efficient
// to use _mm_addsub_pd and _mm_hadd_pd if SSE3 is available.
static inline __v2Cxs operator / (__v2Cxs const & a,
__v2Cxs const & b) {
   __m128 a_flipped;                 // (a1.re,a1.im,a0.re,a0.im)   a =(a1.im,a1.re,a0.im,a0.re)
   __m128 b_re;                      // (b1.re, b1.re,b0.re, b0.re)
   __m128 b_im;                      // (b1.im,-b1.im,b0.im,-b0.im)
   __m128 ab_conj;                   // a * conjugate(b)
   __m128 abs_b_square;              // (b1.im*b1.im,b1.re*b1.re,b0.im*b0.im,b0.re*b0.re)
   __m128 abs2_b;                    //+(b1.re*b1.re,b1.im*b1.im,b0.re*b0.re,b0.im*b0.im)
   __m128 rcp_abs2_b;

   abs_b_square = _mm_mul_ps(b, b);   // 
   // Reciprocal of horizontal add:
   
   abs2_b =   _mm_add_ps( abs_b_square, 
			  _mm_shuffle_ps(abs_b_square, abs_b_square,_MM_SHUFFLE(2,3,0,1) ));
 
   rcp_abs2_b = _mm_rcp_ps(abs2_b);//rsqrt is very fast, don't worry about it
   // The following code is made as similar to the operator * as
   // possible, to enable common subexpression elimination in code
   // that contains both operator * and operator / with the same
   // second operand:
   static const union {               // (signbit,0)
      int i[4]; __m128 v;
   } signbitlow = {0x80000000,0,0x80000000,0};
   b_im = _mm_shuffle_ps(b,b,_MM_SHUFFLE(3,3,1,1));      // Imag. part of b in both
   b_re = _mm_shuffle_ps(b,b,_MM_SHUFFLE(2,2,0,0));      // Real part of b in both
   a_flipped = _mm_shuffle_ps(a,a,_MM_SHUFFLE(2,3,0,1)); // Swap real and imag parts of a
   b_im = _mm_xor_ps(b_im, signbitlow.v); // Change sign of re
   // Multiply and subtract:
   ab_conj = (__v2Cxs)_mm_mul_ps(a, b_re) -
             (__v2Cxs)_mm_mul_ps(a_flipped, b_im);
   // Multiply by rcp_abs2_b:
   return (__v2Cxs) _mm_mul_ps(ab_conj, rcp_abs2_b) ;
}

// - complex: (-a.re, -a.im):
static inline __v2Cxs operator - (__v2Cxs const & a) {
   static const union {               // (signbit,signbit)
      int i[4]; __m128 v;
   } signbits = {0x80000000,0x80000000,0x80000000,0x80000000};
   return (__v2Cxs)_mm_xor_ps(a, signbits.v); // Change sign of both elements
}
// complex conjugate: (a.re, -a.im)
static inline __v2Cxs conj(__v2Cxs const & a) {
   static const union {                // (signbit,signbit)
      int i[4]; __m128 v;
   } signbithigh = {0,0x80000000,0,0x80000000};
   return (__v2Cxs)_mm_xor_ps(a, signbithigh.v); // Change sign of imag. part
}

static inline __v2Cxs abs2(__v2Cxs const & a) {
  __m128 aa;// im1 re1 im0 re0
  __m128 aa_flipped; //-im1 im1 -im0 im0 
  aa = _mm_mul_ps(a,a);
  aa_flipped = _mm_shuffle_ps(aa,aa,_MM_SHUFFLE(3,3,1,1)); 
  static const union {                
      int i[4]; __m128 v;
   } signbithigh = {0,0x80000000,0,0x80000000};
  aa_flipped  = _mm_xor_ps(aa_flipped, signbithigh.v);
  return (__v2Cxs)_mm_add_ps(aa,aa_flipped);
}

static inline __v2Cxs normcx(__v2Cxs const & a) {
  __m128 aa;             // im1 re1 im0 re0
  __m128 aa_flipped;     // re1 im1 re0 im0
  __m128 abs2_a;         // aa + aa_flipped 
  __m128 rsqrt_abs2_a;
  aa           = _mm_mul_ps(a,a);
  aa_flipped   = _mm_shuffle_ps(aa,aa,_MM_SHUFFLE(2,3,0,1)); 
  abs2_a       = _mm_add_ps(aa,aa_flipped);
  rsqrt_abs2_a = _mm_rsqrt_ps(abs2_a);//very efficient, checked with rsqrt(Sigfl)
  return (__v2Cxs)_mm_mul_ps(a,rsqrt_abs2_a);
}


//std::ostream& operator << (std::ostream& os,const __v2Cxs& cxval);
// FOR DEBUG
//       float debug[4];
//       _mm_store_ps (debug,vecCx_1);
//       cout<<debug[0]<<" "<<debug[1]<<" "<<debug[2]<<" "<<debug[3];mygetchar();
 
#endif
