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
#define TLOOP(n) for(int il=0;il<n;il++){
#define ENDTLOOP() }
#else
#define TLOOP(n) 
#define ENDTLOOP() 
#endif

#include "func_add_asm.hh" 
#include "func_blas_asm.hh"


/************************************************************************/
#define Size 2048
//NOTE
//Size = 8192: Cache load      45.000cycles
//Size = 1024:                  2.400cycles

//23/08/09 test on Intel Q8200
//2048        : L1(64KBytes)
//4096->8192x8: L2(2MBytes)
//8192x16     : >L2

  float  ALIGNED  A [Size];
  float  ALIGNED  B [Size];
  float  ALIGNED  C [Size];  float C_1 [Size];  float C_2 [Size];

  float  ALIGNED  a [Size] ;//float  x [3] ;
  float  ALIGNED  b [Size] ;//float  y [3] ;
  float  ALIGNED  c [Size] ;//float  z [3] ;

//   float*  a = new float [Size] ;float  x [3] ;
//   float*  b = new float [Size] ;float  y [3] ;
//   float*  c = new float [Size] ;float  z [3] ;

//NOTE:
// +  float* a = new float [Size] ; data non aligné,
//   =>segmentation fault if sse is used
// +  float  a[Size] ;  data est automatiquement aligné 
//   par 32 par compilateur with option O3!!
// +  float ALIGNED a[Size] ;  data est  aligné par 16

int n = Size/4;int nsur4 = Size/16;

/************************************************************************/
void init(){
  
  for(int kk=0;kk<Size;kk++)
    {
      A[kk] = 0.123f + kk; 
      B[kk] = 0.789f + kk;
      //C[kk] = 0.789f + kk; //load C to cache
    }
    //__declspec(align(16))         // align a and b by 16
  TIME();
  for(int kk=0;kk<Size;kk++)
    {
      a[kk] = 0.123f + kk; 
      b[kk] = 0.789f + kk;
      //c[kk] = 0.789f + kk; //load c to cache
    }
  TIME("INIT");

}
/************************************************************************/

//float sdot_C(int n, float  *x,float  *y) { float  s=0.0;    while (n--)     { s+=*(x++) * *(y++); }; return s; }
//void  saxpy_C(int n, float   a,float  *x, float  *y) {       while (n--)  *(y++)+=  a    * *(x++); };
#include "benchTime.h"
void test_time_ms(){
    BenchTime T1;
    BenchTime T2;

    int nbIter = 50000;

    volatile float ref = 0.0f; //avoid optimization, speedup sdot_C, interesting?
    volatile float res = 0.0f;
    
    //-------------------------------------------------//
    T1.start();
    TLOOP(nbIter);
    for(int jj=0;jj<Size;jj +=4){
      C[jj  ] = A[jj  ] + B[jj];
      C[jj+1] = A[jj+1] + B[jj+1];
      C[jj+2] = A[jj+2] + B[jj+2];
      C[jj+3] = A[jj+3] + B[jj+3];
    }
    ENDTLOOP();
    T1.stop();
    ref= sdot_C(Size,C,C);

    T2.start();
    TLOOP(nbIter);
    add_sse_3(a,b,c, Size); 
    ENDTLOOP();
    T2.stop();
    res= sdot_C(Size,c,c);

    cout<<c_blue<<"\nFast ADD operation: ( c[i] =a[i] + b[i]  for_all_i )\n"<<c_normal;
    printf(        " dim   = %i\n"
		   " nIter = %i\n"
		   " Benchmark Timings (all times in milliseconds):\n"
		   ,Size,nbIter); 
    T1.print("  STDC on float     ");
    T2.print("  SSE2 on float     ");
    
    cout<<"ref:"<<ref<<endl;
    cout<<"res:"<<res<<endl;
    //-------------------------------------------------//

    T1.reset();
    T2.reset();
   
    T1.start();
    TLOOP(nbIter);
    ref= sdot_C(Size,a,b);
    ENDTLOOP();
    T1.stop();
    
    T2.start();
    TLOOP(nbIter);
    res = sdot_sse(Size,a,b);
    ENDTLOOP();
    T2.stop();

    cout<<c_blue<<"\nFast SDOT operation: ( ref+=a[i] * b[i]  for_all_i )\n"<<c_normal;
    printf(       " dim   = %i\n"
	          " nIter = %i\n"
	          " Benchmark Timings (all times in milliseconds):\n"
	          ,Size,nbIter); 
    T1.print("  STDC on float     ");
    T2.print("  SSE2 on float     ");
    
    cout<<"ref:"<<ref<<endl;
    cout<<"res:"<<res<<endl;

    //-------------------------------------------------//


    T1.reset();
    T2.reset();

    float acoef = 0.1f;
    
    for(int kk=0;kk<Size;kk++)c[kk] = 0.789f + kk;
    T1.start();
    TLOOP(nbIter);
    saxpy_C(Size,acoef,a,c);
    ENDTLOOP();
    T1.stop();
    
    ref= sdot_C(Size,c,c);

    for(int kk=0;kk<Size;kk++)c[kk] = 0.789f + kk;
    T2.start();
    TLOOP(nbIter);
    //saxpy_sse(Size,acoef,a,c);
    saxpy_sse_intrin(Size,acoef,a,c);
    ENDTLOOP();
    T2.stop();

    res =  sdot_C(Size,c,c);

    cout<<c_blue<<"\nFast SAXPY INTRIN operation: ( c[i] +=acoef[i] * a[i]  for_all_i )\n"<<c_normal;
    printf(       " dim   = %i\n"
	          " nIter = %i\n"
	          " Benchmark Timings (all times in milliseconds):\n"
	          ,Size,nbIter); 
    T1.print("  STDC on float     ");
    T2.print("  SSE2 on float     ");

    cout<<"ref:"<<ref<<endl;
    cout<<"res:"<<res<<endl;
    //-------------------------------------------------//

    T1.reset();
    T2.reset();


    for(int kk=0;kk<Size;kk++)c[kk] = 0.789f + kk;
    T1.start();
    TLOOP(nbIter);
    saxpy_C(Size,acoef,a,c);
    ENDTLOOP();
    T1.stop();
    
    ref= sdot_C(Size,c,c);

    for(int kk=0;kk<Size;kk++)c[kk] = 0.789f + kk;
    T2.start();
    TLOOP(nbIter);
    saxpy_sse(Size,acoef,a,c);
    ENDTLOOP();
    T2.stop();

    res =  sdot_C(Size,c,c);

    cout<<c_blue<<"\nFast SAXPY operation: ( c[i] +=acoef[i] * a[i]  for_all_i )\n"<<c_normal;
    printf(       " dim   = %i\n"
	          " nIter = %i\n"
	          " Benchmark Timings (all times in milliseconds):\n"
	          ,Size,nbIter); 
    T1.print("  STDC on float     ");
    T2.print("  SSE2 on float     ");

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
