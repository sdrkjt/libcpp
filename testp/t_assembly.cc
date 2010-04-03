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
#define Size 8192
//NOTE
//Size = 8192: Cache load      45.000cycles
//Size = 1024:                  2.400cycles

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

 
#define NLOOP 1
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
void test_add_normal()
{
  //   TIME();
  //   for(int jj=0;jj<Size;jj ++){
  //     C[jj  ] = A[jj  ] + B[jj];
  //   }
  //   TIME("ADD_normal no unroll");
  TIME();
  TLOOP(NLOOP);
  for(int jj=0;jj<Size;jj +=4){
    C[jj  ] = A[jj  ] + B[jj];
    C[jj+1] = A[jj+1] + B[jj+1];
    C[jj+2] = A[jj+2] + B[jj+2];
    C[jj+3] = A[jj+3] + B[jj+3];
  }
  ENDTLOOP();
  TIME("ADD_normal");
}

void test_add_asm(){

  TIME();
  TLOOP(NLOOP);
  add_sse(a,b,c,n);
  ENDTLOOP();
  TIME("ADD_asm_sse");
  
  TIME();
  TLOOP(NLOOP);
  add_sse_2(a,b,c,Size);
  ENDTLOOP();
  TIME("ADD_asm_sse_2");

  TIME();
  TLOOP(NLOOP);
  add_sse_3(a,b,c,Size);
  ENDTLOOP();
  TIME("ADD_asm_sse_3");
  
  TIME();
  TLOOP(NLOOP);
  add_sse_16(a,b,c,nsur4);
  ENDTLOOP();
  TIME("ADD_asm_sse_16_movaps");

  for (int i = 0; i < 10; i++)
    cout<<c[i]-b[i];
  float sum = 0;
  for (int i = 0; i < Size; i++)
    sum += c[i]+b[i];
  cout<<sum<<endl;
 }

void test_add_intrin(){

  TLOOP(4);  TIME();
  for (int i = 0; i < Size; i+=4) {
    *((__m128*)(c+i)) = *((__m128*)(a+i)) + *((__m128*)(b+i)) ;
  }
  TIME("test_add_intrin"); ENDTLOOP(); 

}

void test_elt_sig(){
  Sigfl cxa,cxb,cxc;

  cxa.ones(Size);cxb.ones(Size);cxc.SetSize(Size);

  TLOOP(4);
  TIME();
  cxc = cxa + cxb;
  TIME("ADD SIGFL");
  ENDTLOOP();
}

void test_complexd(){
  Sigcxd cxa,cxb,cxc;

  cxa.SetSize(Size);cxb.SetSize(Size);cxc.SetSize(Size);
  for(int kk=0;kk<Size;kk++)
    {
      cxa.val(kk) = _mm_set_pd(kk*0.1,kk*0.7);
      cxb.val(kk) = _mm_set_pd(kk*0.11,kk*0.27);
    }
  cxc = cxa + cxb;
}


void test_complexs(){
  Sigcxs cxa,cxb,cxc;

  cxa.SetSize(Size);cxb.SetSize(Size);cxc.SetSize(Size);
  //cxa.SigStep(0,1,Size);
  for(int kk=0;kk<Size;kk++)
    {
      cxa.val(kk) = Cxs(kk*0.1,kk*0.7);
      cxb.val(kk) = Cxs(kk*0.11,kk*0.27);
    }
  cxc = cxa * cxb;
  //TIME();
}
void test_rsqrt(){
  Sigfl cxa,cxb,cxc;
  cxb.ones(Size);
  cxc = rsqrt(cxb);
};
void test_sqrt(){
  Sigfl cxa,cxb,cxc;
  cxb.ones(Size);
  cxc = sqrt(cxb);
};

void test_xor(){
  Sigui8 cxa,cxb,cxc;
  cxa.random(Size);
  cxb.random(Size);
  cxc.SetSize(Size);
  TLOOP(10);
  TIME();
  cxc = cxa ^ cxb;
  //for(int ii=0;ii<Size;ii++)
  // cxc(ii) = cxa(ii) ^ cxb(ii);
  TIME("XOR");
  ENDTLOOP();

  DUMP(cxc);
}

//NOTE 21/02/2009://///////////////////////////////////////////////////
// add_sse_16 nhanh hon add_sse khoang 1000cycles (8192)
// dung intrinsic cho cung performance nhu asm inline (avec -O3)
// without -O3: intrinsic 36000, asm inline 19000 
// dung sse nhanh hon 6000cycles so voi ko dung sse (8192): 25000-19000
//
//NOTE 28/03/2009
//test_add_normal() vs test_add_asm()
//**first call***/ need to load data into the cache
//|option| 1024  | 2048 | 8192 |
//|------|-------|------|------|
//|sse2  | 2500  | 10000| 50000|
//|------|-------|------|----- |
//|normal| 8600  | 17000| 60000|
//**second call**/ dont need to load data into the cache
//|option| 1024  | 2048 | 8192 |
//|------|-------|------|------|
//|sse2  | 2400  | 4900 | 19000|
//|------|-------|------|------|
//|normal| 3200  | 6300 | 25000|
////we need 30000cycles to load data(8192) into the cache
//**mean/1000 calls**/ dont need to load data into the cache
//|option| 1024  | 2048 | 8192 |
//|------|-------|------|------|
//|sse2  | 1100  | 2100 | 25000|
//|------|-------|------|------|
//|normal| 2100  | 4150 | 42000|
//EXPERIENCE WITH MEMCPY
//41000 cycles first call (with 3versions: memcpy of libc, mymemcpy_sse, mymemcpy_movsb)
//11000 cycles second call
//non aligned data:  memcpy of libc(24300) > agner fog(13700)
////////////////////////////////////////////////////////////////////////
//toto_alignedarray
//tab[8192]      17900 cycles
//new char[...]  21000 cycles
//
//class example  20000 cycles


//NOTE 18/04/09
//with option O3: inline automatique, meme si la fonction est définie sans le mot inline
//le prolème ne se produit pas si l'on n'utilise pas l'option O3 
//bug détecté avec SDOT: elle est inlinée par O3, et le code compilé ne marche pas
//si l'on definit la fonction avec .hh et .cc, la fonction sera compilée à part, donc pas
//de inline automatique avec O3

//NOTE 19/04/09
//éviter d'utiliser les registres généraux explicitement dans le code sans les déclarer 
//dans les 2 listes à la fin du code, sinon beaucoup de problèmes peuvent se produire
//ex: loop infini, ... (voir  add_sse_3 in t_add_asm.cc)

int main(){
  OVERHEAD();
 
  init();
  test_add_intrin();
  //test_add_asm();
  //test_add_normal();
  //test_elt_sig();
  
  //test_complexd();
  //test_complexs();
  //test_rsqrt();
  //test_sqrt();
  //test_xor();
  return 1;
}
