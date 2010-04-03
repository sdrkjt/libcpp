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
#if !check_asm
#define LOOP(n) for(int il=0;il<n;il++){
#define ENDLOOP() }
#else
#define LOOP(n) 
#define ENDLOOP() 
#endif

/************************************************************************/
#define Size 2048

/************************************************************************/

/************************************************************************/
#include "benchTime.h"

#define STACK(var) cout<<"stack**" << #var <<":"<<(int)(&var)<<endl;
#define memsize   3000000
char* AdrPool;
char* CurPos;

int main(){
  OVERHEAD(); 

  char ALIGNED megalibpool[memsize];
  memset(megalibpool,0,memsize);
  
  char* pstack; pstack = megalibpool;

  getchar();

  STACK(megalibpool);

  /*--------Alloc----------------*/
  float* adra = (float*)pstack; //STACK(adra);
    
  pstack += Size*4;
  
  float* adrb = (float*)pstack; //STACK(adrb);
  
  pstack += Size*4;
  
  float* adrc = (float*)pstack; //STACK(adrc);
  
  /*--------Init-----------------*/

  for(int kk=0;kk<Size;kk++)
    {
      *(adra + kk) = 0.123f + kk; 
      *(adrb + kk) = 0.789f + kk;
      //*(adrc + kk) = 0.789f + kk; //load c to cache
    }

  /*-----------------------------*/

  LOOP(4);
  TIME();
  for(int ii = 0;ii<Size;ii += 4)			 
    *((__m128*)(adrc+ii)) = *((__m128*)(adra+ii)) +  *((__m128*)(adrb+ii)) ;
  TIME("ADD");
  ENDLOOP();

  float checksum = 0;
  for(int ii=0;ii<Size;ii++)checksum+= *(adrc+ii);
  cout<<"check sum: "<<checksum<<endl;
    

  BenchTime T1;
  BenchTime T2;
  int nbIter = 50000;
  T2.start();
  LOOP(nbIter);
  for(int ii = 0;ii<Size;ii += 4)			 
    *((__m128*)(adrc+ii)) = *((__m128*)(adra+ii)) +  *((__m128*)(adrb+ii)) ;
  ENDLOOP();
  T2.stop();
 
  printf("\nFast ADD operation: ( c[i] =a[i] + b[i]  for_all_i )\n"
	 " dim   = %i\n"
	 " nIter = %i\n"
	 " Benchmark Timings (all times in milliseconds):\n"
            ,Size,nbIter); 
  T1.print("  Sligth Compute on float     ");
  T2.print("  Heavy  Compute on float     ");

  return 1;
}
