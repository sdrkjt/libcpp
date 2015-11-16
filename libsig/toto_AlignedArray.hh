#ifndef testclass_hh
#define testclass_hh

#include <memory.h>                              // For memcpy and memset
#include <stdlib.h>                              // For exit in Error function
#include "cpp_monit.hh"

//#define BOUNDSCHECKING 0                        // 0 will skip array bounds checking

#ifdef BOUNDSCHECKING_
#define val(i) data(i)             //use checking out of range
#else
#define val(i) bufferA[i]          //without checking out of range, for speedup
#endif

// #ifndef BOUNDSCHECKING_
// #define VECT(Sig,type,i) *(type*)(Sig.bufferA + i) //without checking out of range, for speedup
// #else 
// #define VECT(Sig,type,i) Sig.GetVect(i)      //use checking out of range
// #endif

#define testAATV testclass<TX,VY>

#define use_static_alloc

template <typename TX>
class testbuffer {
public:
  //char * name;
  
  int MaxSize;                                  // Maximum number of objects that buffer can contain
  char * bufferU;                               // Unaligned data buffer
  //float* bufferA;                               // Aligned pointer to data buffer  
  int ok;
  int      nbref;                               // Current number of signals that point to this buffer

  testbuffer(){
    //name = "noname";
    //bufferU = 0;bufferA = 0; MaxSize=0; nbref = -1111;
  }
  //   ~testbuffer(){
  //     //cout<<c_red<<"delete buffer: "<<name<<c_normal<<endl;
  //     if (bufferU) delete[] bufferU;
  //   }
};
#include <emmintrin.h>  
// Class testclass makes an aligned dynamic array
template <typename TX, typename VY>
class testclass {
public:
  int nb_parallel;
#ifdef use_static_alloc
  char tab[8192*160];//  __attribute__((  aligned(16)  ));
#endif
  TX  * bufferA;
  char * toto;  

  //testbuffer<TX>* buf;
  //TX * perturbateur;                           
  //  char* name;
  
  // Constructor
  testclass();
  // Destructor
  //   ~testclass();
  // Set the number of valid entries. New entries will be zero
  void SetSize(int num);
  void ones(int _size) { 
    SetSize(_size);
    for(int ii=0;ii<_size;ii++)*(bufferA+ii) = 1;
  }
  void zeros(int _size) { 
    SetSize(_size);
    for(int ii=0;ii<_size;ii++)*(bufferA+ii) = 0;
  }
  //    //Reference
  //    void reference(const testAATV& Sin);
  //    void ones (int num){
  //      if(NumEntries!=0){cout<<c_red<<"Error: Signal is already allocated"<<c_normal<<endl; myabort();}
  //      this->SetSize(num); *this = 1;}
  //    void zeros(int num){
  //      if(NumEntries!=0){cout<<c_red<<"Error: Signal is already allocated"<<c_normal<<endl; myabort();}
  //      this->SetSize(num); *this = 0;}
  //    void rand(const int &num){
  //      //if(NumEntries!=0){cout<<c_red<<"Error: Signal is already allocated"<<c_normal<<endl; myabort();}
  //      randomInit(*this,num);//defined in sig_init.hh
  //    };
  //    void rand_wgn(const int &num);
  //    // Allocate buffer for future growth to num objects
  //    void Reserve(int num);
  //    // Get number of objects in array
  int GetSize() const{return NumEntries;};
  //    // Get number of objects that can be stored without re-allocating memory
  //    int GetMaxSize(){return buf->MaxSize;};
  //    // Access object with index i (use val(i) for speedup)
  //    TX&  data(int i) {
  //      if ((unsigned int)i >= (unsigned int)NumEntries) {Error(1, i); i = 0;} // Index i out of range
  //      return bufferA[i];
  //    }
  
  //   // Access vector starting at index i
  //    VY& GetVect(int i) {
  //       const int ElementsPerVector = sizeof(VY) / sizeof(TX);// Number of elements per vector
  //       if (i & (ElementsPerVector-1)) {Error(2, i); i = 0;} // Index i not divisible by vector size
  //       if ((unsigned int)(ElementsPerVector - 1 + i) >= (unsigned int)NumEntries) {Error(1, i);  i = 0;}  // Index i out of range
  //       return *(VY*)(bufferA + i);
  //    }
  // Define desired alignment. Must be a power of 2:
  enum DefineSize {
    AlignBy = 16                               // Desired alignment, must be a power of 2
  };

  //    void operator = (testclass const& Sig_src);
  //    void operator = (TX const& Scalar);
  //    void operator = (OP<TX,VY> const& op) 
  //    {
  //      op.exec(*this);// exec must be const
  //    }
  //    //void operator =(RES<testAATV> const& res){
  //    //  this->reference(res.sig);
  //    //}
   
private:
  //char * bufferU;                              
  //int nb_ref;
  //int MaxSize;                                  // Maximum number of objects that buffer can contain
  //   testclass(testclass const&){};          // Make private copy constructor to prevent copying
  //void operator = (testclass const&){};      // Make private assignment operator to prevent copying
protected:
  int NumEntries;                               // Number of objects stored
  void Error(int e, int n);                     // Make fatal error message

};
// #include <emmintrin.h>  
// #include <emmintrin.h>  

// #define AlignBy 16
// template <typename TX,typename VY>
// class testclass {
// public:
//   TX * bufferA;
//   int size;
//   testclass(){bufferA = 0;};
//   void SetSize(int num);
//   void ones(int _size) { 
//     SetSize(_size);
//     *(bufferA+0) = 1.0f;*(bufferA+1) = 1.0f;*(bufferA+2) = 1.0f;*(bufferA+3) = 1.0f;
//     for(int ii=0;ii<size;ii+=4)*((__m128*)(bufferA+ii)) = *((__m128*)(bufferA));
//   }
//   int GetSize(){ return size;}
// };

// template <typename TX,typename VY>
// void testclass<TX,VY>::SetSize(int num){
//   char * buffer2U = 0;                          // New buffer, unaligned
//   // Aligned pointer to new buffer:
//   union {
//     char * b;                                  // Used for converting from char*
//     TX * p;                                    // Converted to TX *
//     long int i;                                // Used for alignment
//   } buffer2A;
   
//   buffer2U = new char[num*sizeof(TX)+AlignBy-1];// Allocate new buffer

//   buffer2A.b = buffer2U + AlignBy - 1;
   
//   buffer2A.i &= - (long int)AlignBy;

//   bufferA = buffer2A.p;
// }
//MUST INCLUDE .cc TO DEFINE TEMPLATE


#include "toto_AlignedArray.cc"


#endif
