#define check_asm 0
//g++ -S -O3 -msse2 t_complex.cc
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
#define TLOOP(n) for(int il=0;il<n;il++){
#define ENDTLOOP() }
#else
#define TLOOP(n) 
#define ENDTLOOP() 
#endif

char * AllocMem(int numElt){
  char * ptr = new char[numElt + 16];
  int offset = (int64)ptr & 0xf;
  if(offset)
    ptr = (char*)( (uint64)ptr + 16 - offset );
  return  ptr;
};

/************************************************************************/
#define Size 8192//1024
enum{MUL_Cxs, MUL_Cxd};

#define testId 1

#if(testId==1)
#define OPNAME "MUL Cxs"
#define OPID   *
#define CType  Complexs
#define SType  Sigcxs
#define Cxds   Cxs
#else 
#if(testId==2)
#define OPNAME "MUL Cxd"
#define OPID   *
#define CType  Complexd
#define SType  Sigcxd
#define Cxds   Cxd
#else
#if(testId==3)
#define OPNAME "ADD Cxs"
#define OPID   +
#define CType  Complexs
#define SType  Sigcxs
#define Cxds   Cxs
#else
#if(testId==4)
#define OPNAME "ADD Cxd"
#define OPID   +
#define CType  Complexd
#define SType  Sigcxd
#define Cxds   Cxd
#endif//4
#endif//3
#endif//2
#endif//1

CType ALIGNED  A [Size];
CType ALIGNED  B [Size];
CType ALIGNED  C [Size]; 

//CType * A,*B,*C ;


SType cxa,cxb,cxc;

/************************************************************************/
void init(){
  char *test = new char[16];
  cxa.SetSize(Size);cxb.SetSize(Size);cxc.SetSize(Size);
  //cxa.SigStep(0,1,Size);
  for(int kk=0;kk<Size;kk++)
    {
      cxa.val(kk) = Cxds(0.123f + kk,0.456f + kk);
      cxb.val(kk) = Cxds(0.789f + kk,0.123f + kk);
    }

  //   A = (CType *) AllocMem(Size*sizeof(CType));
  //   B = (CType *) AllocMem(Size*sizeof(CType));
  //   C = (CType *) AllocMem(Size*sizeof(CType));
  for(int kk=0;kk<Size;kk++)
    {
      A[kk] = CType(0.123f + kk,0.456f + kk) ; 
      B[kk] = CType(0.789f + kk,0.123f + kk);
    }


}
/************************************************************************/

//float sdot_C(int n, float  *x,float  *y) { float  s=0.0;    while (n--)     { s+=*(x++) * *(y++); }; return s; }
//void  saxpy_C(int n, float   a,float  *x, float  *y) {       while (n--)  *(y++)+=  a    * *(x++); };
#include "benchTime.h"
void test_time_ms(){
    BenchTime T1;
    BenchTime T2;

    int nbIter = 50000;

    volatile float ref = 0.0f; 
    volatile float res = 0.0f;
    
    //-------------------------------------------------//
    T1.start();
    TLOOP(nbIter);
    for(int jj=0;jj<Size;jj ++)
     C[jj  ] = A[jj  ] OPID B[jj];
    ENDTLOOP();
    T1.stop();
    ref= real(C[111]);

    T2.start();
    TLOOP(nbIter);
    cxc = cxa OPID cxb;
    ENDTLOOP();
    T2.stop();
    res= cxc.val(111).Re();
    
    printf("\nFast %s complex operation: ( c[i] =a[i] %s b[i]  for_all_i )\n"
	   " dim   = %i\n"
	   " nIter = %i\n"
	   " Benchmark Timings (all times in milliseconds):\n"
	   ,OPNAME,OPNAME,Size,nbIter);
    
    T1.print("  STDC      ");
    T2.print("  SSE2      ");
    
    cout<<"ref:"<<ref<<endl;
    cout<<"res:"<<res<<endl;
    //-------------------------------------------------//
}

int main(){
  OVERHEAD();
 
  init();

  test_time_ms();

  return 1;
}
