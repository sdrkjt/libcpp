#include "func_add_asm.hh"

//NOTE:
//2cycles/emptyloop 

/*SIMDx86 STYLE*/
void add_sse(float* a_, float*b_, float* c_, int n){
  asm __volatile__(
		   "add_sse%=:\n"
		   "movaps	(%0), %%xmm0\n"
		   "addps	(%1), %%xmm0\n"
		   "movaps	%%xmm0, (%2)\n"
		   "addq	$16, %0  \n" /*64bit*/
		   "addq	$16, %1  \n"
		   "addq	$16, %2  \n"
		   "decl        %3      \n"
		   "cmpl	$0, %3  \n"
		   "jne add_sse%=\n"
		   :
		   : "r" (a_), "r" (b_), "r" (c_), "r" (n)
		   );
}

/*libSIMD STYLE*/
void add_sse_2(float* a_, float*b_, float* c_,int n){
  asm __volatile__(
		   "xorl	%%eax, %%eax \n"
		   "add_sse_2%=:\n"
		   "movaps	(%%edi,%%eax,4), %%xmm0 \n"
		   "addps	(%%esi,%%eax,4), %%xmm0 \n"
		   "movaps	%%xmm0, (%%edx,%%eax,4) \n"
		   "addl	$4, %%eax \n"
		   "cmpl	%%ebx, %%eax \n"
		   "jne add_sse_2%=\n"
		   :
		   :"S" (a_), "D" (b_), "d" (c_), "b" (n)
		   :"memory"
		   );
}
/*Agner Fog Style*/
void  add_sse_3(float*src1,float*src2,float* dest, int size)  {
  __asm__  __volatile__(
			//"mov %[src1] , %%esi \n"
			//"mov %[src2] , %%edi \n"
			//"mov %[dest] , %%edx \n"
			//"mov %[n]    , %%ecx \n"
			"lea (%%esi,%%ecx,4), %%esi \n"
			"lea (%%edi,%%ecx,4), %%edi \n"
			"lea (%%edx,%%ecx,4), %%edx \n"
			"neg  %%ecx     \n"
			
			"add_sse_3%=:\n"
			"movaps   (%%esi,%%ecx,4), %%xmm0 \n"
			"addps    (%%edi,%%ecx,4), %%xmm0 \n"
			"movaps    %%xmm0,   (%%edx,%%ecx,4) \n" 
			"add	$4,   %%ecx \n"
			"jnz add_sse_3%=\n"
			:
			//:[dest] "m" (dest), [src1] "m" (src1),[src2] "m" (src2),[n] "m" (size)
			//:"%edi","%esi","%edx","%ecx","memory" //withoud this, loop infini detected: 
			//loop(3) { add_sse_3 }
			:"d" (dest), "S" (src1),"D" (src2),"c" (size)
			:"memory" //dont need "%edi" in this list
			);
}
void add_sse_16(float* a_, float*b_, float* c_, int n){
  asm __volatile__(
		   "simd_array1_a_add_l1%=:\n"
 		   "movaps   (%0), %%xmm0\n"
 		   "movaps   (%1), %%xmm4\n"
		   "addps %%xmm0 , %%xmm4\n"
		   "movaps %%xmm4,   (%2)\n"
		   " "
		   "movaps 16(%0), %%xmm1\n"
		   "movaps 16(%1), %%xmm5\n"
		   "addps %%xmm1, %%xmm5\n"
		   "movaps %%xmm5, 16(%2)\n"
		   " "
 		   "movaps 32(%0), %%xmm2\n"
 		   "movaps 32(%1), %%xmm6\n"	
		   "addps %%xmm2, %%xmm6\n"
		   "movaps %%xmm6, 32(%2)\n"
		   " "
		   "movaps 48(%0), %%xmm3\n"
		   "movaps 48(%1), %%xmm7\n"	
		   "addps %%xmm3, %%xmm7\n"
		   "movaps %%xmm7, 48(%2)\n"
		   "add	$64, %0  \n"
		   "add	$64, %1  \n"
		   "add	$64, %2  \n"
		   "dec        %3      \n"
		   "cmp	$0, %3  \n"
		   "jne simd_array1_a_add_l1%=\n"
		   :
		   : "r" (a_), "r" (b_), "r" (c_), "r" (n)
		   );
}
