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

#define ALIGNED __attribute__((  aligned(16)  ))
#if !check_asm
#define LOOP(n) for(int il=0;il<n;il++){
#define ENDLOOP() }
#else
#define LOOP(n) 
#define ENDLOOP() 
#endif

typedef double vecd3[3];
typedef double vecd2[2];

typedef struct vecs3
{
  float x, y, z;
  float u; /*Padding to 16th byte for SIMD operations*/
} vecs3 ALIGNED;

typedef struct vec32
{
  float x[32];
} vec32 ALIGNED;

// The Gnu style inline assembly does not allow you to put the names of local C++ variables
// directly into the assembly code. Only global variable names will work because they have the
// same names in the assembly code. Instead you can use the so-called extended syntax as
// illustrated above. The extended syntax looks like this:
// __asm__ ("assembly code string" : [output list] : [input list] :
// [clobbered registers list] ); optimizing_assembly p.40

vec32 va,vb,vc;
int vecSz = 32;int vn = 32/16;
void init(){
  for(int ii=0;ii<vecSz;ii++)
    {
      va.x[ii] = 0.1f;
      vb.x[ii] = ii;
      vc.x[ii] = 0.0f;
    }
}

void simd_vecs3_add(vecs3* pOut, const vecs3* pIn1, const vecs3* pIn2)
{
  /* SSE/SSE2/SSE3 Implementation */
  asm __volatile__(
		   "movaps (%0), %%xmm0\n"
		   "movaps (%1), %%xmm1\n"
		   "addps %%xmm0, %%xmm1\n"
		   "movaps %%xmm1, (%2)\n"	
		   :
		   : "r" (pIn1), "r" (pIn2), "r" (pOut)
		   );
}
void test_simd_vecs3_add(){
  vecs3 Sa,Sb,Sc;
  Sb.x=1.1;Sb.y=1.2;Sb.z=1.3; Sb.u=1.5;
  Sa.x=2.1;Sa.y=2.2;Sa.z=2.3; Sa.u=2.5;
  Sc.x=2.1;Sc.y=2.2;Sc.z=2.3; Sc.u=3.5;

  for(int ii=0;ii<10;ii++){
    //TIME();
    simd_vecs3_add(&Sc,&Sa,&Sb);
    //TIME("simd_vecs3_add");
  }
}
void add_sse_vec(vec32* a_, vec32 *b_, vec32 * c_, int n){
  asm __volatile__(
		   "simd_array1_a_add_l1%=:\n"
		   "movaps   (%0), %%xmm0\n"
		   "movaps 16(%0), %%xmm1\n"
		   "movaps 32(%0), %%xmm2\n"
		   "movaps 48(%0), %%xmm3\n"
		   "movaps   (%1), %%xmm4\n"
		   "movaps 16(%1), %%xmm5\n"	
		   "movaps 32(%1), %%xmm6\n"	
		   "movaps 48(%1), %%xmm7\n"	
		   "addps %%xmm0, %%xmm4\n"    
		   "addps %%xmm1, %%xmm5\n"
		   "addps %%xmm2, %%xmm6\n"
		   "addps %%xmm3, %%xmm7\n"
		   "movaps %%xmm4,   (%2)\n"   
		   "movaps %%xmm5, 16(%2)\n"
		   "movaps %%xmm6, 32(%2)\n"
		   "movaps %%xmm7, 48(%2)\n"
		   "addl	$64, %0  \n"
		   "addl	$64, %1  \n"
		   "addl	$64, %2  \n"
		   "decl        %3      \n"
		   "cmpl	$0, %3  \n"
		   "jne simd_array1_a_add_l1%=\n"
		   :
		   : "r" (a_), "r" (b_), "r" (c_), "r" (n)
		   );
}


void test_vec(){
  add_sse_vec(&va,&vb,&vc,vn);
  TIME();
  add_sse_vec(&va,&vb,&vc,vn);
  TIME("ADD_sse_vec32");
  
  for(int ii=0;ii<vecSz;ii++)
   cout<<vc.x[ii]<< "  ";

}
int main(){
  OVERHEAD();
 
  test_vec();
  return 1;
}
