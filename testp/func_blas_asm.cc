#include "func_blas_asm.hh"
#ifndef inliner
float   sdot_C(int n, float  *x,float  *y) { float  s=0.0;    while (n--)     { s+=*(x++) * *(y++); }; return s; }
#endif

float  sdot_sse(int   nv, float  *xv, float  *yv)
{ 

  float res; res = 0.0f;

  __asm__ __volatile__ (							
		    "            xorps     %%xmm7, %%xmm7                             \n "
		    "            mov       %%ecx,%%edx    /* %%ecx = %%edx = n/4 */   \n "
		    "            movaps    %%xmm7,%%xmm0                              \n "
		    "            movaps    %%xmm7,%%xmm1                              \n "
		    "            movaps    %%xmm7,%%xmm2                              \n "
		    "            movaps    %%xmm7,%%xmm3                              \n "
		    "            movaps    %%xmm7,%%xmm4                              \n "
		    "            movaps    %%xmm7,%%xmm5                              \n "
		    "            movaps    %%xmm7,%%xmm6                              \n "
		    "            shrl       $2,%%edx                                  \n "
		    "            andl       $0x07,%%edx                               \n "
		    "            je        kk1%=                                      \n "
		    "            movaps    (%%eax),%%xmm6                             \n "
		    "            mulps     (%%ebx),%%xmm6                             \n "
		    "            cmp       $3,%%edx                                   \n "
		    "            jg        kkg3%=                                     \n "
		    "            jne       kkg9%=                                     \n "
		    "            movaps    0x10(%%eax),%%xmm5                         \n "
		    "            mulps     0x10(%%ebx),%%xmm5                         \n "
		    "            movaps    0x20(%%eax),%%xmm4                         \n "
		    "            mulps     0x20(%%ebx),%%xmm4                         \n "
		    "            jmp       kk0%=                                      \n "
		    " kkg9%=:  cmp       $2,%%edx                                     \n "
		    "            jl        kk0%=                                      \n "
		    "            movaps    0x10(%%eax),%%xmm5                         \n "
		    "            mulps     0x10(%%ebx),%%xmm5                         \n "
		    "            jmp       kk0%=                                      \n "
		    " kkg3%=: movaps    0x30(%%eax),%%xmm3                            \n "
		    "            movaps    0x20(%%eax),%%xmm4                         \n "
		    "            mulps     0x30(%%ebx),%%xmm3                         \n "
		    "            movaps    0x10(%%eax),%%xmm5                         \n "
		    "            mulps     0x20(%%ebx),%%xmm4                         \n "
		    "            mulps     0x10(%%ebx),%%xmm5                         \n "
		    "            cmp       $5,%%edx                                   \n "
		    "            jg        kkg5%=                                     \n "
		    "            jne       kk0%=                                      \n "
		    "            movaps    0x40(%%eax),%%xmm2                         \n "
		    "            mulps     0x40(%%ebx),%%xmm2                         \n "
		    "            jmp       kk0%=                                      \n "
		    " kkg5%=: movaps    0x50(%%eax),%%xmm1                            \n "
		    "            mulps     0x50(%%ebx),%%xmm1                         \n "
		    "            movaps    0x40(%%eax),%%xmm2                         \n "
		    "            mulps     0x40(%%ebx),%%xmm2                         \n "
		    "            addps     %%xmm1,%%xmm3                              \n "
		    "            cmp       $6,%%edx                                   \n "
		    "            je        kk0%=                                      \n "
		    "            movaps    0x60(%%eax),%%xmm0                         \n "
		    "            mulps     0x60(%%ebx),%%xmm0                         \n "
		    "            addps     %%xmm0,%%xmm2                              \n "
		    " kk0%=:  shll       $4,%%edx                                     \n "
		    "            addl       %%edx,%%ebx                               \n "
		    "            addl       %%edx,%%eax                               \n "
		    " kk1%=:  mov       %%ecx,%%edx                                   \n "
		    "            shrl       $5,%%ecx                                  \n "
		    "            je        kmp3%=                                     \n "
		    "            movaps    (%%eax),%%xmm0                             \n "
		    "            movaps    0x10(%%eax),%%xmm1                         \n "
		    " .p2align 4    /* each loop does 32 add+multiply */              \n "
		    " kmp2%=: mulps     (%%ebx),%%xmm0                                \n "
		    "            addps     %%xmm2,%%xmm6                              \n "
		    "            movaps    0x20(%%eax),%%xmm2                         \n "
		    "            mulps     0x10(%%ebx),%%xmm1                         \n "
		    "            addps     %%xmm3,%%xmm7                              \n "
		    "            movaps    0x30(%%eax),%%xmm3                         \n "
		    "            mulps     0x20(%%ebx),%%xmm2                         \n "
		    "            addps     %%xmm0,%%xmm4                              \n "
		    "            movaps    0x40(%%eax),%%xmm0                         \n "
		    "            mulps     0x30(%%ebx),%%xmm3                         \n "
		    "            addps     %%xmm1,%%xmm5                              \n "
		    "            movaps    0x50(%%eax),%%xmm1                         \n "
		    "            mulps     0x40(%%ebx),%%xmm0                         \n "
		    "            addps     %%xmm2,%%xmm6                              \n "
		    "            movaps    0x60(%%eax),%%xmm2                         \n "
		    "            mulps     0x50(%%ebx),%%xmm1                         \n "
		    "            addps     %%xmm3,%%xmm7                              \n "
		    "            movaps    0x70(%%eax),%%xmm3                         \n "
		    "            mulps     0x60(%%ebx),%%xmm2                         \n "
		    "            addps     %%xmm0,%%xmm4                              \n "
		    "            movaps    0x80(%%eax),%%xmm0                         \n "
		    "            mulps     0x70(%%ebx),%%xmm3                         \n "
		    "            addps     %%xmm1,%%xmm5                              \n "
		    "            movaps    0x90(%%eax),%%xmm1                         \n "
		    "            add	   $0x80,%%ebx                                \n "
		    "            add	   $0x80,%%eax                                \n "
		    "            dec	   %%ecx                                      \n "
		    "            jne	   kmp2%=                                     \n "
		    " kmp3%=: addps     %%xmm2,%%xmm6                                 \n "
		    "            addps     %%xmm3,%%xmm7                              \n "
		    "            addps     %%xmm5,%%xmm4                              \n "
		    "            addps     %%xmm6,%%xmm7                              \n "
		    "            addps     %%xmm7,%%xmm4                              \n "
		    "            xorps     %%xmm0,%%xmm0                              \n "
		    "            movhlps   %%xmm4,%%xmm0                              \n "
		    "            addps     %%xmm4,%%xmm0                              \n "
		    "            movaps    %%xmm0,%%xmm1                              \n "
		    "            shufps    $0xE1,%%xmm4,%%xmm1                        \n "
		    "            addss     %%xmm1, %%xmm0                             \n "
		    "            andl      $3,%%edx                                   \n "
		    "            je       kmp4%=                                      \n "
		    "            movss    (%%eax),%%xmm1                              \n "
		    "            mulss    (%%ebx),%%xmm1                              \n "
		    "            addss    %%xmm1,%%xmm0                               \n "
		    "            dec      %%edx                                       \n "
		    "            je       kmp4%=                                      \n "
		    "            movss    4(%%eax),%%xmm2                             \n "
		    "            mulss    4(%%ebx),%%xmm2                             \n "
		    "            addss    %%xmm2,%%xmm0                               \n "
		    "            dec      %%edx                                       \n "
		    "            je       kmp4%=                                      \n "
		    "            movss    8(%%eax),%%xmm3                             \n "
		    "            mulss    8(%%ebx),%%xmm3                             \n "
		    "            addss    %%xmm3,%%xmm0                               \n "
		    " kmp4%=: movss    %%xmm0, (%0)                                   \n "
		    : 
		    : "S" (&res),"c" (nv), "a" (xv), "b" (yv)                 
		    : "%edx", "memory" 
									);
  return res;
}


#ifndef inliner
void saxpy_C(int n, float   a,float  *x, float  *y) {       while (n--)  *(y++)+=  a    * *(x++); };
#endif

void saxpy_sse(int np,const float& ap, float* xp, float* yp){
  asm __volatile__ (	
		    "             movss   (%0),%%xmm7     /* %%xmm5 holds a for use below */   \n"
		    "             shufps   $0,%%xmm7,%%xmm7 /* %%xmm5 holds a for use below */ \n"
		    "             mov      %%ecx,%%edi      /* %%ecx = %%ebx = n/4 */          \n"
		    "             shrl       $2,%%edi                                          \n"
		    "             andl       $0x07,%%edi                                       \n"
		    "             je        ll3%=                                              \n"
		    "             movaps    (%%eax),%%xmm6                                     \n"
		    "             mulps     %%xmm7,%%xmm6                                      \n"
		    "             addps     (%%ebx),%%xmm6                                     \n"
		    "             movaps    %%xmm6,(%%ebx)                                     \n"
		    "             cmp       $3,%%edi                                           \n"
		    "             jg        ll1%=                                              \n"
		    "             jne        ll9%=                                             \n"
		    "             movaps    0x10(%%eax),%%xmm5                                 \n"
		    "             mulps     %%xmm7,%%xmm5                                      \n"
		    "             addps     0x10(%%ebx),%%xmm5                                 \n"
		    "             movaps    %%xmm5,0x10(%%ebx)                                 \n"
		    "             movaps    0x20(%%eax),%%xmm4                                 \n"
		    "             mulps     %%xmm7,%%xmm4                                      \n"
		    "             addps     0x20(%%ebx),%%xmm4                                 \n"
		    "             movaps    %%xmm4,0x20(%%ebx)                                 \n"
		    "             jmp        ll3%=                                             \n"
		    " ll9%=  :    cmp       $2,%%edi                                           \n"
		    "             jl        ll3%=                                              \n"
		    "             movaps    0x10(%%eax),%%xmm5                                 \n"
		    "             mulps     %%xmm7,%%xmm5                                      \n"
		    "             addps     0x10(%%ebx),%%xmm5                                 \n"
		    "             movaps    %%xmm5,0x10(%%ebx)                                 \n"
		    "             jmp        ll3%=                                             \n"
		    " ll1%=  :   movaps    0x30(%%eax),%%xmm3                                  \n"
		    "             mulps     %%xmm7,%%xmm3                                      \n"
		    "             movaps    0x20(%%eax),%%xmm4                                 \n"
		    "             addps     0x30(%%ebx),%%xmm3                                 \n"
		    "             movaps    %%xmm3,0x30(%%ebx)                                 \n"
		    "             mulps     %%xmm7,%%xmm4                                      \n"
		    "             movaps    0x10(%%eax),%%xmm5                                 \n"
		    "             addps     0x20(%%ebx),%%xmm4                                 \n"
		    "             movaps    %%xmm4,0x20(%%ebx)                                 \n"
		    "             mulps     %%xmm7,%%xmm5                                      \n"
		    "             addps     0x10(%%ebx),%%xmm5                                 \n"
		    "             movaps    %%xmm5,0x10(%%ebx)                                 \n"
		    "             cmp       $5,%%edi                                           \n"
		    "             jg        ll2%=                                              \n"
		    "             jne        ll3%=                                             \n"
		    "             movaps    0x40(%%eax),%%xmm2                                 \n"
		    "             mulps     %%xmm7,%%xmm2                                      \n"
		    "             addps     0x40(%%ebx),%%xmm2                                 \n"
		    "             movaps    %%xmm2,0x40(%%ebx)                                 \n"
		    "             jmp       ll3%=                                              \n"
		    " ll2%=  :   movaps    0x50(%%eax),%%xmm1                                  \n"
		    "             mulps     %%xmm7,%%xmm1                                      \n"
		    "             movaps    0x40(%%eax),%%xmm2                                 \n"
		    "             addps     0x50(%%ebx),%%xmm1                                 \n"
		    "             movaps    %%xmm1,0x50(%%ebx)                                 \n"
		    "             mulps     %%xmm7,%%xmm2                                      \n"
		    "             addps     0x40(%%ebx),%%xmm2                                 \n"
		    "             movaps    %%xmm2,0x40(%%ebx)                                 \n"
		    "             cmp       $6,%%edi                                           \n"
		    "             je        ll3%=                                              \n"
		    "             movaps    0x60(%%eax),%%xmm0                                 \n"
		    "             mulps     %%xmm7,%%xmm0                                      \n"
		    "             addps     0x60(%%ebx),%%xmm0                                 \n"
		    "             movaps    %%xmm0,0x60(%%ebx)                                 \n"
		    " ll3%=  :   shll       $4,%%edi                                           \n"
		    "             subl       $0x40,%%edi                                       \n"
		    "             addl       %%edi,%%eax                                       \n"
		    "             addl       %%edi,%%ebx                                       \n"
		    "             mov       %%ecx,%%edi                                        \n"
		    "             shrl       $5,%%ecx          /* n/32 */                      \n"
		    "             je        ll6%=              /* if zero, take short path */  \n"
		    "             movaps    0x60(%%eax),%%xmm2                                 \n"
		    "             movaps    0x50(%%eax),%%xmm1                                 \n"
		    "             mulps     %%xmm7,%%xmm2                                      \n"
		    "             movaps    0x40(%%eax),%%xmm0                                 \n"
		    "             mulps     %%xmm7,%%xmm1                                      \n"
		    "             addps     0x60(%%ebx),%%xmm2                                 \n"
		    "             jmp       ll5%=                                              \n"
		    " .p2align 4                                                               \n"
		    " ll4%=  :   movaps    0x30(%%eax),%%xmm3                                  \n"
		    "             mulps     %%xmm7,%%xmm0                                      \n"
		    "             movaps    %%xmm2,0x20(%%ebx)                                 \n"
		    "             addps     0x10(%%ebx),%%xmm1                                 \n"
		    "             movaps    0x60(%%eax),%%xmm2                                 \n"
		    "             mulps     %%xmm7,%%xmm3                                      \n"
		    "             movaps    %%xmm1,0x10(%%ebx)                                 \n"
		    "             addps     (%%ebx),%%xmm0                                     \n"
		    "             movaps    0x50(%%eax),%%xmm1                                 \n"
		    "             mulps     %%xmm7,%%xmm2                                      \n"
		    "             movaps    %%xmm0,(%%ebx)                                     \n"
		    "             addps     0x30(%%ebx),%%xmm3                                 \n"
		    "             movaps    0x40(%%eax),%%xmm0                                 \n"
		    "             mulps     %%xmm7,%%xmm1                                      \n"
		    "             movaps    %%xmm3,0x30(%%ebx)                                 \n"
		    "             addps     0x60(%%ebx),%%xmm2                                 \n"
		    " ll5%=  :   movaps    0x70(%%eax),%%xmm3                                  \n"
		    "             mulps     %%xmm7,%%xmm0                                      \n"
		    "             movaps    %%xmm2,0x60(%%ebx)                                 \n"
		    "             addps     0x50(%%ebx),%%xmm1                                 \n"
		    "             movaps    0xA0(%%eax),%%xmm2                                 \n"
		    "             mulps     %%xmm7,%%xmm3                                      \n"
		    "             movaps    %%xmm1,0x50(%%ebx)                                 \n"
		    "             addps     0x40(%%ebx),%%xmm0                                 \n"
		    "             movaps    0x90(%%eax),%%xmm1                                 \n"
		    "             mulps     %%xmm7,%%xmm2                                      \n"
		    "             movaps    %%xmm0,0x40(%%ebx)                                 \n"
		    "             addps     0x70(%%ebx),%%xmm3                                 \n"
		    "             movaps    0x80(%%eax),%%xmm0                                 \n"
		    "             mulps     %%xmm7,%%xmm1                                      \n"
		    "             movaps    %%xmm3,0x70(%%ebx)                                 \n"
		    "             addps     0xA0(%%ebx),%%xmm2                                 \n"
		    "             add	    $0x80,%%eax                                        \n"
		    "             add	    $0x80,%%ebx                                        \n"
		    "             dec       %%ecx                                              \n"
		    "             jne	    ll4%=                                              \n"
		    "             movaps    0x30(%%eax),%%xmm3                                 \n"
		    "             mulps     %%xmm7,%%xmm0                                      \n"
		    "             movaps    %%xmm2,0x20(%%ebx)                                 \n"
		    "             addps     0x10(%%ebx),%%xmm1                                 \n"
		    "             mulps     %%xmm7,%%xmm3                                      \n"
		    "             movaps    %%xmm1,0x10(%%ebx)                                 \n"
		    "             addps     (%%ebx),%%xmm0                                     \n"
		    "             movaps    %%xmm0,(%%ebx)                                     \n"
		    "             addps     0x30(%%ebx),%%xmm3                                 \n"
		    "             movaps    %%xmm3,0x30(%%ebx)                                 \n"
		    " ll6%=  :    andl      $3,%%edi                                           \n"
		    "             je       ll7%=                                               \n"
		    "             movss    0x40(%%eax),%%xmm4                                  \n"
		    "             mulss    %%xmm7,%%xmm4                                       \n"
		    "             addss    0x40(%%ebx),%%xmm4                                  \n"
		    "             movss    %%xmm4,0x40(%%ebx)                                  \n"
		    "             dec      %%edi                                               \n"
		    "             je       ll7%=                                               \n"
		    "             movss    0x44(%%eax),%%xmm4                                  \n"
		    "             mulss    %%xmm7,%%xmm4                                       \n"
		    "             addss    0x44(%%ebx),%%xmm4                                  \n"
		    "             movss    %%xmm4,0x44(%%ebx)                                  \n"
		    "             dec      %%edi                                               \n"
		    "             je       ll7%=                                               \n"
		    "             movss    0x48(%%eax),%%xmm4                                  \n"
		    "             mulss    %%xmm7,%%xmm4                                       \n"
		    "             addss    0x48(%%ebx),%%xmm4                                  \n"
		    "             movss    %%xmm4,0x48(%%ebx)                                  \n"
		    " ll7%=  :                                                                 \n"
		    : : "q" (&ap), "a" (xp), "b" (yp), "c" (np)                                
		    : "xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7","edi","memory","cc" 
			);
}
#define  motif_saxpy(ii,tmp1) __m128 tmp1 =  tmpA * *((__m128*)(xp+ii)) ; \
    *((__m128*)(yp+ii)) =  _mm_add_ps(tmp1, *((__m128*)(yp+ii))); \


void saxpy_sse_intrin(int np,const float& ap, float* xp, float* yp){
  __m128 tmpA; tmpA =  _mm_set1_ps(ap);
  
  for(int ii=0;ii<np;ii+=32){
    motif_saxpy(ii,tmp1);
    motif_saxpy(ii+4,tmp2);
    motif_saxpy(ii+8,tmp3);
    motif_saxpy(ii+12,tmp4);
    motif_saxpy(ii+16,tmp5);
    motif_saxpy(ii+20,tmp6);
    motif_saxpy(ii+24,tmp7);
    motif_saxpy(ii+28,tmp8);
  }
}
#define  motif_sdot(ii,tmp1,tmp1A) tmp1 =   _mm_mul_ps( *((__m128*)(xv+ii)), *((__m128*)(yv+ii)) ) ; \
    tmp1A =  _mm_add_ps(tmp1, tmp1A);					\

float  sdot_sse_intrin(int   nv, float  *xv, float  *yv)
{ 
  __m128 tmp1A,tmp2A,tmp3A,tmp4A,tmp5A,tmp6A,tmp7A,tmp8A;
  __m128 tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8;

  tmp1A = _mm_xor_ps(tmp1A,tmp1A); 
  tmp2A = _mm_xor_ps(tmp2A,tmp2A);
  tmp3A = _mm_xor_ps(tmp3A,tmp3A);
  tmp4A = _mm_xor_ps(tmp4A,tmp4A);
  //tmp5A = _mm_xor_ps(tmp5A,tmp5A);
  //tmp6A = _mm_xor_ps(tmp6A,tmp6A);
  //tmp7A = _mm_xor_ps(tmp7A,tmp7A);
  //tmp8A = _mm_xor_ps(tmp8A,tmp8A);

  for(int ii=0;ii<nv;ii+=16){
    motif_sdot(ii   ,tmp1,tmp1A);
    motif_sdot(ii+4 ,tmp2,tmp2A);
    motif_sdot(ii+8 ,tmp3,tmp3A);
    motif_sdot(ii+12,tmp4,tmp4A);
    //motif_sdot(ii+16,tmp5,tmp5A);
    //motif_sdot(ii+20,tmp6,tmp6A);
    //motif_sdot(ii+24,tmp7,tmp7A);
    //motif_sdot(ii+28,tmp8,tmp8A);
  }
  __m128 tmpA;
  tmpA = _mm_add_ps(tmpA,tmp1A);
  tmpA = _mm_add_ps(tmpA,tmp2A);
  tmpA = _mm_add_ps(tmpA,tmp3A);
  tmpA = _mm_add_ps(tmpA,tmp4A);
  //tmpA = _mm_add_ps(tmpA,tmp5A);
  //tmpA = _mm_add_ps(tmpA,tmp6A);
  //tmpA = _mm_add_ps(tmpA,tmp7A);
  //tmpA = _mm_add_ps(tmpA,tmp8A);

  __m128 tmpB;  //_mm_xorps(tmpB,tmpB);
  tmpB = _mm_movehl_ps(tmpB,tmpA);	// tmpB = 0 0 d1 d2
  tmpB = _mm_add_ps(tmpB,tmpA);         // tmpB = d1 d2 d1+d3 d2+d4
  __m128 tmpB_shuffle;
  tmpB_shuffle = _mm_shuffle_ps(tmpB,tmpB,_MM_SHUFFLE(3,2,0,1)); //tmpB_shuffle =   d1 d2 d2+d4 d1+d3
  tmpB = _mm_add_ss(tmpB,tmpB_shuffle);

  float res;
  _mm_store_ss(&res,tmpB);


  //float* toto = (float*)&tmpA;
  //return toto[0]+toto[1]+toto[2]+toto[3];

  return res;
}
