template <typename TX,typename VY>
int AATV::nb_parallel = sizeof(VY) / sizeof(TX);

template <typename TX,typename VY>
int AATV::counter = 0;

// Members of class AlignedArray
template <typename TX,typename VY>
AATV::AlignedArray() { 
   // Constructor
  bufferA = 0; buf = new buffer<TX>();
  UsefulDataSize = 0;
#ifdef SigName
  name = "S"+num2str(counter);
  buf->name = name;  // Buffername is the name of original signal
#endif
  //  cerr<<"new sig: "<<name<<endl;
  counter++;
}
template <typename TX,typename VY>
AATV::AlignedArray(buffer<TX> *buf_in, TX *bufferA_in, int UsefulDataSize_in){
  buf            = buf_in;
  bufferA        = bufferA_in;
  UsefulDataSize = UsefulDataSize_in;
#ifdef SigName
  name           = string("cast_")+buf->name;
#endif
  buf->nbref++;
};
template <typename TX,typename VY>
AATV::~AlignedArray() {
  DeleteBuffer();
}
template <typename TX,typename VY>
void AATV::DeleteBuffer() {
  // Destructor
  int*  nbref = &(buf->nbref);
  (*nbref)--;
  if(*nbref==0) { 
    //cerr<<"delete signal buffer: "<<buf->name<<endl;
    delete buf;
  }
  else if(*nbref<0) {
    cout<<c_red<<"Error: Sig Destructor uncorrect"<<c_normal<<endl;myabort();
  }
}
template <typename TX,typename VY>
void AATV::reference(const AATV& Sin){
  //remove the old buffer
  DeleteBuffer(); 
  //connect to the new buffer 
  buf            = Sin.buf; 
  bufferA        = Sin.bufferA;
  UsefulDataSize = Sin.UsefulDataSize;
#ifdef SigName
  name           = string("ref_")+Sin.name;
#endif
  buf->nbref++;
}
// template <typename TX,typename VY>
// void AATV::copyref(const AATV& Sin){
//    this->reference(Sin);
//    UsefulDataSize = Sin.UsefulDataSize;
// }
template <typename TX,typename VY>
void AATV::Malloc(int num) {
   // Allocate buffer for future growth to num objects.
   // Use this if it can be predicted that the size will be increased 
   // later with SetSize(num). This will minimize the number of
   // memory re-allocations.
   //
   // Setting num > current BufSize will allocate a larger buffer and 
   // move all data to the new buffer.
   //
   // Setting num <= current BufSize will do nothing. The buffer will 
   // only grow, not shrink.
   //
   // Setting num = 0 will discard all data and de-allocate the buffer.
   if (num <= buf->BufSize) {
      if (num <= 0) {
         if (num < 0) Error(1, num);
         // num = 0. Discard data and de-allocate buffer
	 delete buf;
	 UsefulDataSize = 0;
         return;
      }
      // Request to reduce size. Ignore
      return;
   }
   // num > BufSize. Allocate new buffer
   char * buffer2U = 0;                          // New buffer, unaligned
   // Aligned pointer to new buffer:
   union {
      char * b;                                  // Used for converting from char*
      TX * p;                                    // Converted to TX *
      long int i;                                // Used for alignment
   } buffer2A;

   // Note: On big-endian platforms buffer2A.i must have the same size as a pointer,
   // on little-endian platforms it doesn't matter.
   /*aligner les 2 extremités du buffer pour faciliter les calculs SIMD
     voir sig_eltwise
    */
   //buffer2U = new char[num*sizeof(TX)+AlignSize-1];// Allocate new buffer
   buffer2U = memalloc<char>(num*sizeof(TX)+15+(AlignSize-1));

   if (buffer2U == 0) {Error(3,num); return;}    // Error can't allocate
   // Align new buffer by AlignSize (must be a power of 2)
   buffer2A.b = buffer2U + AlignSize - 1;
   
   buffer2A.i &= - (long int)AlignSize;
 
   if (buf->bufferA) {
      // A smaller buffer is previously allocated
      memcpy(buffer2A.p, buf->bufferA, UsefulDataSize*sizeof(TX));// Copy contents of old buffer into new one
      //delete[] (buf->bufferU);                          // De-allocate old buffer
      memfree(buf->bufferU); myabort();
   }
   buf->bufferU = buffer2U;                           // Save pointer to buffer
   buf->bufferA = buffer2A.p;                         // Save aligned pointer to new buffer
   buf->BufSize = num;                                // Save new size
   bufferA = buf->bufferA;

   //cout<<c_red<<"new buffer alloc "<<buf->name<<" size "<<num<<c_normal<<endl;

}


template <typename TX,typename VY>
void AATV::setsize(int num) {
   // Set the number of objects that are considered used and valid.
   // UsefulDataSize is initially zero. It is increased by Push or SetSize
   // Setting num > UsefulDataSize is equivalent to pushing (num - UsefulDataSize)
   // objects with zero contents.
   // Setting num < UsefulDataSize will decrease UsefulDataSize so that all objects 
   // with index >= num are erased.
   // Setting num = 0 will erase all objects, but not de-allocate the buffer.
  
   if (num < 0) { // Cannot be negative
      Error(1, num); return;
   }
   if (num > buf->BufSize) {
      // Allocate larger buffer. 
      Malloc(num);
   }
   if (num > UsefulDataSize) {
      // Fill new entries with zero
      //memset(bufferA + UsefulDataSize, 0, (num - UsefulDataSize) * sizeof(TX));
   }
   // Set new DataSize
   UsefulDataSize = num;
}
  
// Produce fatal error message. Used internally and by StringElement.
// Note: If your program has a graphical user interface (GUI) then you
// must rewrite this function to produce a message box with the error message.
template <typename TX,typename VY>
void AATV::Error(int e, int n) const{
   // Define error texts
   static const char * ErrorTexts[] = {
      "Unknown error",                           // 0
      "Index out of range",                      // 1
      "Index not divisible by vector size",      // 2
      "Memory allocation failed"                 // 3
   };
   // Number of texts in ErrorTexts
   const unsigned int NumErrorTexts = sizeof(ErrorTexts) / sizeof(*ErrorTexts);

   // check that index is within range
   if ((unsigned int)e >= NumErrorTexts) e = 0;

   // Replace this with your own error routine, possibly with a message box:
   fprintf(stderr, "\nAlignedArray error: %s (%i)\n", ErrorTexts[e], n);
   
   // Terminate execution
   //exit(1); 
   myabort();
}

template <typename TX,typename VY>
void AATV::operator = (AlignedArray const& Sig_src){
  int size_src = Sig_src.GetSize();					
  int size_dst = this->GetSize();					
  if(size_dst == 0)						
    {								
      this->SetSize(size_src); 
      size_dst = size_src;					
    }	
#ifdef BOUNDSCHECKING							
  if(size_src!=size_dst)						
    {								
      cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	
      myabort();						
    };		
#endif					
  // TX* adrSrc = Sig_src.bufferA;					 
  // TX* adrDst = this->bufferA;					 
  // int sse_size_1 = sizeof(VY) - 1;				 
  // bool unaligned = (  (((long int)adrSrc)&sse_size_1)||(((long int)adrDst)&sse_size_1) );	 
  // WarningUnaligned(unaligned);

  // int nb_iter   = size_src/nb_parallel;			
  // int nb_entier = unaligned ? 0 : b_iter*nb_parallel;	
 
  // if(unaligned==0){
  //   int ii = 0;							
  //   //if(nb_iter>0)						
  //   for(ii = 0;ii<nb_entier;ii += nb_parallel){		
  //     VECT2((*this),adrDst,VY,ii) =  VECT2(Sig_src,adrSrc,VY,ii);		
  //     /*VECT((*this),VY,ii) =  VECT(Sig_src,VY,ii);*/
  //   }
  //   //if(nb_entier<size_src)					
  //   for(ii = nb_entier;ii<size_src;ii++ )			
  //     (*this)[ii] = Sig_src[ii];
  // }
  // else
  memcpy(this->bufferA,Sig_src.bufferA,size_src*sizeof(TX));
  //mygetchar();
};

template <typename TX,typename VY>
void AATV::operator = (TX const& Scalar){
  int vecSize = this->GetSize();
#ifdef BOUNDSCHECKING						
  if(vecSize == 0)							
    {									
      cout<<c_red<<"Error: Sig Size undefined"<<c_normal<<endl;
      myabort();					
    };		
#endif							
  /*Code Not optimal here*/						
  AATV VScalar; VScalar.SetSize(nb_parallel);			
  for(int ii=0;ii<nb_parallel;ii++)VScalar[ii] = Scalar;	
  /*---------------------*/
  TX* adrDst = this->bufferA;						
  bool unaligned = ((long int)adrDst)&(sizeof(VY)-1);
  WarningUnaligned(unaligned);
  int nb_iter    = vecSize/nb_parallel;				
  int nb_entier;
#ifdef STDCpp
  nb_entier = 0;
#else
  nb_entier = unaligned ? 0 : nb_iter*nb_parallel;
#endif
  int ii = 0;								
  //if(nb_iter>0)							
  for(ii = 0;ii<nb_entier;ii += nb_parallel)			
    VECT((*this),VY,ii) =  VECT(VScalar,VY,0);	
  //if(nb_entier<vecSize)						
  for(ii = nb_entier;ii<vecSize;ii++ )				
    (*this)[ii] = Scalar;				
}

#include "sig_complex.hh"
#include "sig_complexd.hh"

static inline Cxd data2cx(const double & val){
  return Cxd(val,0);
};

static inline Cxd data2cx(const Cxd & val){
  return val;
};
template<typename TX>
static inline Cxd data2cx(const Cx<TX> & val){
  return Cxd(val.Re(),val.Im());
};

template <typename TX,typename VY>
std::ostream& operator << (std::ostream& os,const AATV& sig){
  int sigsize = sig.GetSize();

  // if(sig.nb_parallel>2)
  //   {
  //     cout<<"[ ";
  //     printf("%12.4f",data2float(sig.val(0)));
  //     if(sigsize<8)
  // 	{
  // 	  for(int ii=1;ii<sigsize;ii++)
  // 	    printf(" , %12.4f",data2float(sig[ii]));
  // 	}
  //     else
  // 	{
  // 	  for(int ii=1;ii<4;ii++)
  // 	    printf(" , %12.4f",data2float((sig[ii])));
  // 	  cout<<" , ...";
	
  // 	  for(int ii=sigsize-4;ii<sigsize;ii++)
  // 	    printf(" , %12.4f",data2float((sig[ii])));
  // 	}
  // }
  // else//complex
    {
      cout<<"[ ";
      cout<<data2cx(sig[0]);
    if(sigsize<8)
      {
      for(int ii=1;ii<sigsize;ii++)
	cout<<data2cx(sig[ii]);
      }
    else
      {
	for(int ii=1;ii<4;ii++)
	  cout<<data2cx(sig[ii]);
	cout<<" , ...";
	
	for(int ii=sigsize-4;ii<sigsize;ii++)
	  cout<<data2cx(sig[ii]);
      }
    }
  cout<<"  ]"<<" size = "<<sigsize <<" x 1\n";
}

