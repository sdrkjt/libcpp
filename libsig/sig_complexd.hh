#ifndef sig_complexd_hh
#define sig_complexd_hh

#include <emmintrin.h>
#ifdef SSE3
#include <pmmintrin.h>
#endif
#include <math.h> //for sqrt
#include "cpp_monit.hh"

// Define class Cxd for vector of two doubles.
// The low element represents the real part and the high element
// represents the imaginary part of a complex number:
class __attribute__ (( aligned(16) ))        // must be aligned
Cxd {
   protected:
   __m128d vec;              // Vector of two double's
   public:
   // Default constructor:
   Cxd() {};
   // Constructor to make from real and imaginary part:
   Cxd(double const& re, double const& im) {
      vec = _mm_set_pd(im, re);}
   // Constructor to convert from real number. Set imag. part to 0:
   Cxd(double const x) {
      vec = _mm_set_pd(0., x);}
   // Constructor to convert from type __m128d used in intrinsics:
   Cxd(__m128d const & x) {vec = x;}
   // Operator to convert to __m128d used in intrinsics
   operator __m128d() const {return vec;} //equivalent to *((__m128d*)this)
   // Member function to extract real part:
   double Re() const {
      return *(double const*)&vec;}
   // Member function to extract Imaginary part:
   double Im() const {
     __m128d tmp = _mm_shuffle_pd(vec, vec, 1);
      return *(double const*)&tmp;}
};

// Define operators for class Cxd:
// complex + real: add only to real part:
static inline Cxd operator + (Cxd const &a, double const b){
   return _mm_add_sd(a, _mm_load_sd(&b));}
// complex - real: subtract only from real part:
static inline Cxd operator - (Cxd const &a, double const b){
   return _mm_sub_sd(a, _mm_load_sd(&b));}
// complex * real: multiply both real and imaginary part:
static inline Cxd operator * (Cxd const &a, double const b){
   return _mm_mul_pd(a, _mm_set1_pd(b));}
// complex / real: multiply both real and imag. part by reciprocal b:
static inline Cxd operator / (Cxd const &a, double const b){
   return _mm_mul_pd(a, _mm_set1_pd(1. / b));}
// complex + complex: add both parts:
static inline Cxd operator + (Cxd const & a,
Cxd const & b) {
   return _mm_add_pd(a, b);}
// complex - complex: subtract both parts:
static inline Cxd operator - (Cxd const & a,
Cxd const & b) {
   return _mm_sub_pd(a, b);}

// complex * complex: (a.re*b.re-a.im*b.im, a.re*b.im+b.re*a.im).
// This version is for SSE2 instruction set. It is more efficient
// to use _mm_addsub_pd and _mm_hadd_pd if SSE3 is available.
#ifdef SSE2
static inline Cxd operator * (Cxd const & a,
Cxd const & b) {
   __m128d a_flipped;                 // (a.im,a.re)
   __m128d b_re;                      // (b.re,b.re)
   __m128d b_im;                      // (-b.im,b.im)
   static const union {               // (signbit,0)
      int i[4]; __m128d v;
   } signbitlow = {0,0x80000000,0,0};
   b_im = _mm_shuffle_pd(b,b,3);      // Imag. part of b in both
   b_re = _mm_shuffle_pd(b,b,0);      // Real part of b in  both
   a_flipped = _mm_shuffle_pd(a,a,1); // Swap real and imag parts of a
   b_im = _mm_xor_pd(b_im, signbitlow.v); // Change sign of low
   // Multiply and add:
   return (Cxd)_mm_mul_pd(a, b_re) +
          (Cxd)_mm_mul_pd(a_flipped, b_im);
}
#else //!msse2
#ifdef SSE3
static inline Cxd operator * (Cxd const & a,
Cxd const & b) {
   __m128d a_flipped;                 // (a.im,a.re)
   __m128d b_re;                      // (b.re,b.re)
   __m128d b_im;                      // (b.im,b.im)

   b_im = _mm_shuffle_pd(b,b,3);      // Imag. part of b in both
   b_re = _mm_shuffle_pd(b,b,0);      // Real part of b in  both
   a_flipped = _mm_shuffle_pd(a,a,1); // Swap real and imag parts of a

   // Multiply and add: add MSB, sub LSB
   return _mm_addsub_pd( _mm_mul_pd(a, b_re), _mm_mul_pd(a_flipped, b_im) ); 
}
#else //!msse3
static inline Cxd operator * (Cxd const & a,
			      Cxd const & b) { myabort();return Cxd(-1111,-1111);};
#endif //msse3
#endif //msse2


// complex / complex:
// (a.re*b.re+a.im*b.im, b.re*a.im-a.re*b.im)/(b.re*b.re+b.im*b.im).
// This version is for SSE2 instruction set. It is more efficient
// to use _mm_addsub_pd and _mm_hadd_pd if SSE3 is available.
static inline Cxd operator / (Cxd const & a,
Cxd const & b) {
   __m128d a_flipped;                 // (a.im,a.re)
   __m128d b_re;                      // (b.re,b.re)
   __m128d b_im;                      // (-b.im,b.im)
   __m128d ab_conj;                   // a * conjugate(b)
   __m128d abs_b_square;              // (b.re*b.re,b.im*b.im)
   __m128d abs_bb_shuffle;
   double b_abs_square_recip;         // 1/(abs(b*b))
   abs_b_square = _mm_mul_pd(b, b);   // (b.re*b.re,b.im*b.im)
   // Reciprocal of horizontal add:

   abs_bb_shuffle = _mm_add_sd(abs_b_square,
		       _mm_shuffle_pd(abs_b_square, abs_b_square, 1));
   b_abs_square_recip = 1. /
      *(double const*)& abs_bb_shuffle;
   // The following code is made as similar to the operator * as
   // possible, to enable common subexpression elimination in code
   // that contains both operator * and operator / with the same
   // second operand:
   static const union {               // (signbit,0)
      int i[4]; __m128d v;
   } signbitlow = {0,0x80000000,0,0};
   b_im = _mm_shuffle_pd(b,b,3);      // Imag. part of b in both
   b_re = _mm_shuffle_pd(b,b,0);      // Real part of b in both
   a_flipped = _mm_shuffle_pd(a,a,1); // Swap real and imag parts of a
   b_im = _mm_xor_pd(b_im, signbitlow.v); // Change sign of re
   // Multiply and subtract:
   ab_conj = (Cxd)_mm_mul_pd(a, b_re) -
             (Cxd)_mm_mul_pd(a_flipped, b_im);
   // Multiply by b_abs_square_recip:
   return (Cxd)ab_conj * b_abs_square_recip;
}
// - complex: (-a.re, -a.im):
static inline Cxd operator - (Cxd const & a) {
   static const union {               // (signbit,signbit)
      int i[4]; __m128d v;
   } signbits = {0,0x80000000,0,0x80000000};
   return _mm_xor_pd(a, signbits.v); // Change sign of both elements
}
// complex conjugate: (a.re, -a.im)
static inline Cxd conj(Cxd const & a) {
   static const union {                // (signbit,signbit)
      int i[4]; __m128d v;
   } signbithigh = {0,0,0,0x80000000};
   return _mm_xor_pd(a, signbithigh.v); // Change sign of imag. part
}
// abs2: (a.re*a.re + a.im*a.im)
// 120.000 cycles with 8k
static inline Cxd abs2(Cxd const & a) {
  __m128d aa;// im*im re*re
  __m128d aa_flipped; //im*im im*im 
  aa = _mm_mul_pd(a,a);
  aa_flipped = _mm_shuffle_pd(aa,aa,3); 
  static const union {  
    int i[4]; __m128d v;
  } signbithigh = {0,0,0,0x80000000};
  aa_flipped  = _mm_xor_pd(aa_flipped, signbithigh.v);
  return (Cxd)_mm_add_pd(aa,aa_flipped);
}
// normalisation complex
// 900.000 cycles with 8k
// sqrt is so hard here
static inline Cxd normcx(Cxd const & a){
  __m128d aa        ; // im*im re*re
  __m128d aa_flipped; // re*re im*im
  __m128d abs2_aa   ; // (re*re+im*im) in both part
  double  abs2_a    ; // abs2(a)

  aa         = _mm_mul_pd(a,a);
  aa_flipped = _mm_shuffle_pd(aa,aa,_MM_SHUFFLE2(0,1));
  abs2_aa    = _mm_add_pd(aa,aa_flipped);

  abs2_a = *(double const*)& abs2_aa;

  return a / sqrt(abs2_a);
}

ostream& operator << (ostream& os,const Cxd& cxval);

////FOR DEBUG
//double  debug[2];
//_mm_store_pd(debug,aa_flipped);
//cout<< debug[0]<<" "<<debug[1]<<endl; mygetchar();

#endif
