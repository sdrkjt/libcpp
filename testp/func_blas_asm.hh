#include <emmintrin.h>

#define inliner

#ifdef inliner
//inline float   sdot_C(int n, float  *x,float  *y) { float  s=0.0;    while (n--)     { s+=*(x++) * *(y++); }; return s; }
inline void   saxpy_C(int n, float   a,float  *x, float  *y) {       while (n--)  *(y++)+=  a    * *(x++); };
#else
float    sdot_C(int n, float  *x,float  *y); 
void    saxpy_C(int n, float   a,float  *x, float  *y);
#endif

//float   sdot_sse(int   nv, float  *xv, float  *yv);
void    saxpy_sse(int np,const float& ap, float* xp, float* yp);
void saxpy_sse_intrin(int np,const float& ap, float* xp, float* yp) ;
float   sdot_sse_intrin(int   nv, float  *xv, float  *yv);
float  sdot_sse_intrin(int   nv, float  *xv, float  *yv);
