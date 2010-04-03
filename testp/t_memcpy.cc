#define check_asm 0
//g++ -S -O3 -msse2 t_assembly.cc
#if !check_asm
#include "TSC.hh"
#else
#define TIME(x)
#define DUMP(x)
#define OVERHEAD()
#endif
   
#include <emmintrin.h>
#include <math.h>
#include "../libsig/libsig.hh"

#define ALIGNED __attribute__((  aligned(16)  ))
// #if !check_asm
// #define TLOOP(n) for(int il=0;il<n;il++){
// #define ENDTLOOP() }
// #else
// #define TLOOP(n) 
// #define ENDTLOOP() 
// #endif


/************************************************************************/
#define Size 8192
//NOTE
//Size = 8192: Cache load      45.000cycles
//Size = 1024:                  2.400cycles

  float  ALIGNED  a [Size] ; 
  float  ALIGNED  b [Size] ; 
  float  ALIGNED  c [Size] ; 

//   float*  a = new float [Size] ; 
//   float*  b = new float [Size] ; 
//   float*  c = new float [Size] ; 

//NOTE:
// +  float* a = new float [Size] ; data non aligné,
//   =>segmentation fault if sse is used
// +  float  a[Size] ;  data est automatiquement aligné 
//   par 32 par compilateur with option O3!!
// +  float ALIGNED a[Size] ;  data est  aligné par 16

int n = Size/4;int nsur4 = Size/16;

/************************************************************************/
void init(){

  TIME();
  for(int kk=0;kk<Size;kk++)
    {
      a[kk] = 0.123f + kk; 
      b[kk] = 0.789f + kk;
      //c[kk] = 0.789f + kk; //load c to cache
    }
  TIME("INIT");

}

void  mymemcpy_sse(float* dest, float*src,int size)  {
  __asm__  __volatile__(
		   "mov %[n] , %%ecx \n"
		   "mov %[s] , %%esi \n"
		   "mov %[d] , %%edi \n"
		   "lea (%%esi,%%ecx,4), %%esi \n"
		   "lea (%%edi,%%ecx,4), %%edi \n"
 		   "neg  %%ecx     \n"
		   "mymemcpy_sseL1%=:\n"
		   "movaps   (%%esi,%%ecx,4), %%xmm0 \n"
 		   "movaps 16(%%esi,%%ecx,4), %%xmm1 \n"
 		   "movaps    %%xmm0,   (%%edi,%%ecx,4) \n" 		   
 		   "movaps    %%xmm1, 16(%%edi,%%ecx,4) \n"
		   "add	$8,   %%ecx \n"
		   "jnz mymemcpy_sseL1%=\n"
		   :
		   :[d] "m" (dest), [s] "m" (src),[n] "m" (size)
		   :"%edi","%esi","%ecx","memory"
		   );
}
void mymemcpy_movsb( void *dest, void *src,  int n)
{
    __asm__ __volatile__(
			 "cld   \n"
			 "rep   \n"
			 "movsb \n"
			 :
			 :"c" (n), "S" (src), "D" (dest)
			 :"memory"
			 );

}
void test_memcpy()
{
  for(int ii=0;ii<3;ii++)
    {
     
      TIME();
      mymemcpy_sse(c, a, Size);
      TIME("mymemcpy_sse with ASM inline");

#if !check_asm
#ifdef ASM 
      TIME();
      A_memcpy(c, a, Size*4);
      TIME("AgnerFog_memcpy");
#else
      TIME();
      memcpy(c, a, Size*4);
      TIME("memcpy normal");
#endif
#endif

//       TIME();
//       memcpy(c, a, Size*4);
//       TIME("memcpy normal");

      TIME();
      mymemcpy_movsb(c, a, Size*4);
      TIME("mymemcpy_movsb  ASM inline");

      //cout<<c[8191]<<" ";
      //cout<<a[8191]<<endl;
    }
} 

//EXPERIENCE WITH MEMCPY
//41000 cycles first call (with 3versions: memcpy of libc, mymemcpy_sse, mymemcpy_movsb)
//11000 cycles second call
//non aligned data:  memcpy of libc(24300) > agner fog(13700)

int main(){
  OVERHEAD();
 
  init();
  //getchar();
  test_memcpy();

  return 1;
}
