#include "libsig.hh"

/******************************************************************************
Example part. Remove this from final application:
******************************************************************************/

// Here follows a working example of how to use AlignedArray. 
// To run this example, just compile this file for console mode and run it.
// You may play with this example as you like

int main() {
   int i;                                        // Loop counter

   // Define a vector of 4 floats = (1,2,3,4)
   __m128 const f1234 = _mm_setr_ps(1.0f, 2.0f, 3.0f, 4.0f);
   //__m128 const f1234 = _mm_setr_ps(1.0f, 1.0f, 1.0f, 1.0f);

   // Make instance of AlignedArray containing objects of type float:
   Sigfl list;
   Sigfl lista;
   Sigfl listb;

   // Set the number of entries in the array. All are initialized to 0.
   // Make number divisible by the number of elements in a vector (4)

#define Size 1000000
#define Size_4 Size/4

   int Niter = 1000;

   list.SetSize(Size);
   lista.SetSize(Size);
   listb.SetSize(Size);

   float A [Size];
   float B [Size];
   float C [Size];
 
   // Put data into the array by index
   list.val(0) = list.val(1) = list.val(2)  = list.val(3)  = 100.f;
   list.val(4) = list.val(5) = list.val(6)  = list.val(7)  = 200.f;
   list.val(8) = list.val(9) = list.val(10) = list.val(11) = 300.f;

   lista.val(0) = lista.val(1) = lista.val(2)  = lista.val(3)  = 100.f;
   lista.val(4) = lista.val(5) = lista.val(6)  = lista.val(7)  = 200.f;
   lista.val(8) = lista.val(9) = lista.val(10) = lista.val(11) = 300.f;

   listb.val(0) = listb.val(1) = listb.val(2)  = listb.val(3)  = 100.f;
   listb.val(4) = listb.val(5) = listb.val(6)  = listb.val(7)  = 200.f;
   listb.val(8) = listb.val(9) = listb.val(10) = listb.val(11) = 300.f;

//    float* index[Size_4];
//    float* indexa[Size_4];

//    float* curindex =  list.bufferA;
//    float* curindexa =  lista.bufferA;
 
//    for (i = 0; i < Size_4; i ++) {
//      index[i] = curindex; indexa[i] = curindexa;
//      curindex+=4;curindexa+=4;
//    }

 

   int use_vec = 1;

   if(use_vec)
     {

       // Vectorized loop, step size corresponds to vectors of 4 floats
       for(int ii = 0;ii< Niter;ii++)
	 {
	  
	   list = lista + listb;

	   //  for (i = 0; i < Size_4; i ++) {
	   //for (i = 0; i < Size; i +=4)
	     {
	     // list.GetVect<__m128>(i) is the vector (list[i],list[i+1],list[i+2],list[i+3])
	     // i must be divisible by the number of elements in a vector (4 in this case).
	     // _mm_add_ps is an intrinsic function adding two vectors, defined in xmmintrin.h
	     //list.GetVect<__m128>(i) = _mm_add_ps( lista.GetVect<__m128>(i), f1234 );
	     //list.GetVect<__m128>(i) = _mm_add_ps( lista.GetVect<__m128>(i), listb.GetVect<__m128>(i) );

	     //*(__m128*)(index[i]) = _mm_add_ps( *(__m128*)(indexa[i]), f1234 );
	     //*(__m128*)(list.bufferA + i) = _mm_add_ps( *(__m128*)(lista.bufferA + i),  f1234);
	     //*(__m128*)(list.bufferA + i) = _mm_add_ps( *(__m128*)(lista.bufferA + i),  *(__m128*)(listb.bufferA + i));

	     //index+=4;

	     //VECT(list,__m128,i) = _mm_add_ps( VECT(lista,__m128,i), f1234 );

	     }
	 }
     }

   else
     {
       float test = 1.0f;
       if(0){
	 for(int ii = 0;ii< Niter;ii++)
	   {
	     for (i = 0; i < Size; i ++) {
	       // list.GetVect<__m128>(i) is the vector (list[i],list[i+1],list[i+2],list[i+3])
	       // i must be divisible by the number of elements in a vector (4 in this case).
	       // _mm_add_ps is an intrinsic function adding two vectors, defined in xmmintrin.h
	       //list.GetVect<__m128>(i) = _mm_add_ps( list.GetVect<__m128>(i), f1234 );
	       //list[i] = lista[i] + test;
	       list.val(i) = lista.val(i) + test;
	       //list.bufferA[i] = lista.bufferA[i] + test;
	     }
	   }
       }
       else{
	 for(int ii=0;ii<Niter;ii++)
	   {
	     for(int jj=0;jj<Size;jj++)
	       C[jj] = A[jj] + B[jj];
	       //C[jj] = A[jj] + test;
	   }
       }
     }

   // Output all entries
   //for (i = 0; i < list.GetSize(); i++) {
   for (i = 0; i < 12; i++) {
     printf("\n%2i:  %8.2f", i, list.val(i));
   }

   //to test with OPTION -O3
   cout<<C[0]<<endl;cout<<C[Size-1]<<endl;
   return 0;
}
