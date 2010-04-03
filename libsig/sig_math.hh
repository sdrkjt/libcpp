#ifndef sig_math_hh
#define sig_math_hh

#ifndef ASM
 #include <math.h>
#else
 #include "asmlibran.h"
 #define random MersenneRandom
#endif


#include <emmintrin.h>
static inline int lrintf (float const x) 
{
  return _mm_cvtss_si32(_mm_load_ss(&x));
}
static inline int lrint (double const x) {
  return _mm_cvtsd_si32(_mm_load_sd(&x));
}

#endif
