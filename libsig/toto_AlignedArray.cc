// Members of class testclass
template <typename TX,typename VY>
testAATV::testclass() { 
   // Constructor
  bufferA = 0; 
#ifndef use_static_alloc
  //toto = new char[16];//buf = new testbuffer<TX>();
  cout<<"USE DYNAMIC ALLOC"<<endl;mytrace();
#else
  cout<<"USE STATIC  ALLOC"<<endl;mytrace();
#endif
  NumEntries = 0;
  nb_parallel = sizeof(VY) / sizeof(TX);
  // cout<<"nb_parallel"<<(long int)(&nb_parallel)<<endl;
  // //cout<<(long int)tab<<endl;  
  // cout<<"bufferA"<<(long int)(&bufferA)<<endl;
  // cout<<"toto"<<(long int)(&toto)<<endl;
 
  //mygetchar();

}


// template <typename TX,typename VY>
// testAATV::~testclass() {
//   // Destructor
//   int*  nbref = &(buf->nbref);
//   if(*nbref >0) (*nbref)--;
//   if(*nbref==0) delete buf;
//   else if(*nbref<0 && *nbref!=-1111)
//     {cout<<c_red<<"Error: Sig Destructor uncorrect"<<c_normal<<endl;myabort();}
// }
// template <typename TX,typename VY>
// void testAATV::reference(const testAATV& Sin){
//   buf = Sin.buf; bufferA = Sin.bufferA;
//   NumEntries = Sin.NumEntries;

//   buf->nbref++;
//   //  cout<<c_red<<"reference"<<buf->nbref<<c_normal<<endl;
// }
// // template <typename TX,typename VY>
// // void testAATV::copyref(const testAATV& Sin){
// //    this->reference(Sin);
// //    NumEntries = Sin.NumEntries;
// // }
// template <typename TX,typename VY>
// void testAATV::Reserve(int num) {
//    // Allocate buffer for future growth to num objects.
//    // Use this if it can be predicted that the size will be increased 
//    // later with SetSize(num). This will minimize the number of
//    // memory re-allocations.
//    //
//    // Setting num > current MaxSize will allocate a larger buffer and 
//    // move all data to the new buffer.
//    //
//    // Setting num <= current MaxSize will do nothing. The buffer will 
//    // only grow, not shrink.
//    //
//    // Setting num = 0 will discard all data and de-allocate the buffer.
//    if (num <= buf->MaxSize) {
//       if (num <= 0) {
//          if (num < 0) Error(1, num);
//          // num = 0. Discard data and de-allocate buffer
// 	 delete buf;
// 	 NumEntries = 0;
//          return;
//       }
//       // Request to reduce size. Ignore
//       return;
//    }
//    // num > MaxSize. Allocate new buffer
//    char * buffer2U = 0;                          // New buffer, unaligned
//    // Aligned pointer to new buffer:
//    union {
//       char * b;                                  // Used for converting from char*
//       TX * p;                                    // Converted to TX *
//       long int i;                                // Used for alignment
//    } buffer2A;

//    // Note: On big-endian platforms buffer2A.i must have the same size as a pointer,
//    // on little-endian platforms it doesn't matter.
//    buffer2U = new char[num*sizeof(TX)+AlignBy-1];// Allocate new buffer

//    if (buffer2U == 0) {Error(3,num); return;}    // Error can't allocate
//    // Align new buffer by AlignBy (must be a power of 2)
//    buffer2A.b = buffer2U + AlignBy - 1;
   
//    buffer2A.i &= - (long int)AlignBy;
 
//    if (buf->bufferA) {
//       // A smaller buffer is previously allocated
//       memcpy(buffer2A.p, buf->bufferA, NumEntries*sizeof(TX));// Copy contents of old buffer into new one
//       delete[] (buf->bufferU);                          // De-allocate old buffer
//    }
//    buf->bufferU = buffer2U;                           // Save pointer to buffer
//    buf->bufferA = buffer2A.p;                         // Save aligned pointer to new buffer
//    buf->MaxSize = num;                                // Save new size
//    if(buf->nbref==-1111)buf->nbref = 1;
//    bufferA = buf->bufferA;

//    // cout<<c_red<<"new buffer "<<buf->name<<c_normal<<endl;

// }


template <typename TX,typename VY>
void testAATV::SetSize(int num) {
  NumEntries = num;

  char * buffer2U = 0;                          // New buffer, unaligned
  // Aligned pointer to new buffer:
  union {
    char * b;                                  // Used for converting from char*
    TX * p;                                    // Converted to TX *
    long int i;                                // Used for alignment
  } buffer2A;

#ifndef use_static_alloc
  buffer2U = new char[num*sizeof(TX)+AlignBy-1];// Allocate new buffer
#else
  buffer2U = (char*)((int64)(tab+0));
#endif

  buffer2A.b = buffer2U + AlignBy - 1;
   
  buffer2A.i &= - (long int)AlignBy;

  bufferA = buffer2A.p;
}

   
// // Produce fatal error message. Used internally and by StringElement.
// // Note: If your program has a graphical user interface (GUI) then you
// // must rewrite this function to produce a message box with the error message.
// template <typename TX,typename VY>
// void testAATV::Error(int e, int n) {
//    // Define error texts
//    static const char * ErrorTexts[] = {
//       "Unknown error",                           // 0
//       "Index out of range",                      // 1
//       "Index not divisible by vector size",      // 2
//       "Memory allocation failed"                 // 3
//    };
//    // Number of texts in ErrorTexts
//    const unsigned int NumErrorTexts = sizeof(ErrorTexts) / sizeof(*ErrorTexts);

//    // check that index is within range
//    if ((unsigned int)e >= NumErrorTexts) e = 0;

//    // Replace this with your own error routine, possibly with a message box:
//    fprintf(stderr, "\ntestclass error: %s (%i)\n", ErrorTexts[e], n);
   
//    // Terminate execution
//    exit(1);
// }

// template <typename TX,typename VY>
// void testAATV::operator = (testclass const& Sig_src){
//   int size_src = Sig_src.GetSize();					
//   int size_dst = this->GetSize();					
//   if(size_dst == 0)						
//     {								
//       this->SetSize(size_src);					
//       size_dst = size_src;					
//     }								
//   if(size_src!=size_dst)						
//     {								
//       cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	
//       myabort();						
//     };							
  
//   int nb_iter   = size_src/nb_parallel;			
//   int nb_entier = nb_iter*nb_parallel;			
//   int ii = 0;							
//   if(nb_iter>0)						
//     for(ii = 0;ii<nb_entier;ii += nb_parallel)		
//       VECT((*this),VY,ii) =  VECT(Sig_src,VY,ii);		
//   if(nb_entier<size_src)					
//     for(ii = nb_entier;ii<size_src;ii++ )			
//       this->val(ii) = Sig_src.val(ii);

//   //memcpy(this->bufferA,Sig_src.bufferA,size_src*sizeof(TX));
  
// };

// template <typename TX,typename VY>
// void testAATV::operator = (TX const& Scalar){
//   int vecSize = this->GetSize();						
//   if(vecSize == 0)							
//     {									
//       cout<<c_red<<"Error: Sig Size undefined"<<c_normal<<endl;
//       myabort();					
//     };									
//   /*Code Not optimal here*/						
//   testAATV VScalar; VScalar.SetSize(nb_parallel);			
//   for(int ii=0;ii<nb_parallel;ii++)VScalar.val(ii) = Scalar;	
//   /*---------------------*/						
//   int nb_iter   = vecSize/nb_parallel;				
//   int nb_entier = nb_iter*nb_parallel;				
//   int ii = 0;								
//   if(nb_iter>0)							
//     for(ii = 0;ii<nb_entier;ii += nb_parallel)			
//       VECT((*this),VY,ii) =  VECT(VScalar,VY,0);	
//   if(nb_entier<vecSize)						
//     for(ii = nb_entier;ii<vecSize;ii++ )				
//       this->val(ii) = Scalar;				
// }

