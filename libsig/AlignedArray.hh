#ifndef AlignedArray_hh
#define AlignedArray_hh

#include <cstring>                              // For memcpy and memset
#include "cpp_monit.hh"
#include "memalloc.hh"

#ifndef BOUNDSCHECKING
#define VECT(Sig,type,i) *(type*)(Sig.bufferA + i) //without checking out of range, for speedup
#define VECT2(Sig,adrData,type,i) *(type*)(adrData + i) //without checking out of range, for speedup
#else 
#define VECT(Sig,type,i) Sig.GetVect(i)          //checking out of range
#define VECT2(Sig,adrData,type,i) Sig.GetVect(i) //checking out of range
#endif


#ifdef W_UALIGNED
#define WarningUnaligned(val) { if(val)MSG_WARNING("unaligned memory!! ");}
#else 
#define WarningUnaligned(val) 
#endif		

#define AATV AlignedArray<TX,VY>

template <typename TX,typename VY>
class AlignedArray;

template <typename TX,typename VY>
class OP {
public:
  void (*pf2)(AATV&, const AATV&, const AATV&);
  void (*pf1)(AATV&, const AATV&);
  void (*pfS)(AATV&, const AATV&, const TX&);

  AATV opA;
  AATV opB;
  TX   opS; //scalar

  OP( void (*ipf)(AATV&, const AATV&, const AATV&),
      const AATV &OPA, const AATV &OPB )
  {
    pf2 = ipf; pf1 = NULL;pfS = NULL;

    opA.reference(OPA);
    opB.reference(OPB);
  }

  OP( void (*ipf)(AATV&, const AATV&),
      const AATV &OPA )
  {
    pf2 = NULL; pf1 = ipf; pfS = NULL;
    opA.reference(OPA);
  }

  OP( void (*ipf)(AATV&, const AATV&, const TX&),
      const AATV &OPA,  const TX& ScalarVal )
  {
    pf2 = NULL; pf1 = NULL;pfS = ipf;
    opA.reference(OPA);
    opS = ScalarVal;
  }

  void exec(AATV& OPC) const {
    /*why need const?                     */
    /*see operator = (OP<TX,VY> const& op)*/
    if(pf2!=NULL){
      (pf2)(OPC,opA,opB);
    }
    else if(pfS!=NULL){
      (pfS)(OPC,opA,opS);
    }
    else{
      (pf1)(OPC,opA);
    }
  }

  //constructeur de recopie
  OP( OP const & cp){
    pf1 = cp.pf1; pf2 = cp.pf2; pfS = cp.pfS;
    opA.reference(cp.opA); opB.reference(cp.opA); 
  }
};

template <typename Tinput, typename Toutput>
class OPConv {
public:

  void (*pf)(Toutput&, const Tinput&);
  
  Tinput  opIn ;
  
  OPConv( void (*ipf)(Toutput&, const Tinput&),
      const Tinput &OPA )
  {
    pf = ipf;
    opIn.reference(OPA);
  }
  void exec(Toutput& OPC) const {
    /*why need const?                     */
    /*see operator = (OP<TX,VY> const& op)*/
    (pf)(OPC,opIn);
  }

  //constructeur de recopie
  OPConv( OPConv const & cp){
    pf = cp.pf;
    opIn.reference(cp.opIn);
  }
};

template <typename TX>
class buffer {
 public:
  string  name;

  int BufSize;                                  // Maximum number of objects that buffer can contain
  char * bufferU;                               // Unaligned data buffer
  TX   * bufferA;                               // Aligned pointer to data buffer  
  int      nbref;                               // Current number of signals that point to this buffer

  buffer(){
#ifdef SigName
    name = "noname";
#endif
    bufferU = 0;bufferA = 0; BufSize=0; nbref = 1;
  }
  ~buffer(){
    //cout<<c_red<<"delete buffer: "<<name<<c_normal<<endl;
    if (bufferU) memfree(bufferU);//delete[] bufferU;
  }
};

// Class AlignedArray makes an aligned dynamic array
template <typename TX, typename VY>
class AlignedArray {
public:
   static int nb_parallel;
   static int counter;
   
   TX  * bufferA;                                 
   string name;

   // Constructor
   AlignedArray();
   AlignedArray(buffer<TX> *buf_in, TX *bufferA_in, int UsefulDataSize_in);
   // Destructor
   ~AlignedArray();
   void DeleteBuffer();
   //Reference
   void reference(const AATV& Sin);
   //Sig Conversion (Casting)
   template<typename TXX,typename VYY>
   operator AlignedArray<TXX,VYY> () const{
     AlignedArray<TXX,VYY> newsig ((buffer<TXX>* )buf,(TXX*)bufferA,
				  (UsefulDataSize*sizeof(TX))/sizeof(TXX));
     return newsig;
   };
   // Allocate buffer for future growth to num objects
   void Malloc(int num);
   // Get number of objects in array
   int GetSize() const{return UsefulDataSize;};
   int size() const{return UsefulDataSize;};
   // Get number of objects that can be stored without re-allocating memory
   int GetBufSize(){return buf->BufSize;};
   // Access object with index i 
   // argument "this" may be const& 
   TX& operator[](int pos) const {
#ifdef BOUNDSCHECKING
     if ((unsigned int)pos >= (unsigned int)UsefulDataSize) {Error(1, pos);}   
#endif
     return bufferA[pos];
   }
   AlignedArray operator()(int pos) const {
     AlignedArray tmp; tmp.reference(*this);
     tmp.bufferA       += pos;
     tmp.UsefulDataSize = 1;
#ifdef SigName
     tmp.name           = string("s")+tmp.name;//naming as sref
#endif
     myabort();
     return tmp;
   }
   // Access vector starting at index i
   VY& GetVect(int i) const {
      if (i & (nb_parallel-1)) 
	{Error(2, i);  i = 0;}  // Index i not divisible by vector size
      if ((unsigned int)(nb_parallel - 1 + i) >= (unsigned int)UsefulDataSize) 
	{Error(1, i);  i = 0;}  // Index i out of range
      return *(VY*)(bufferA + i);
   }
   // Define desired alignment. Must be a power of 2:
   enum DefineSize {
     /* Desired alignment, must be a power of 2
	cache line size is 64, so we use 64 by default
     */
      AlignSize = 64
   };

   void operator = (AlignedArray const& Sig_src);
   void operator = (TX const& Scalar);
   void operator = (OP<TX,VY> const& op) 
   {
     op.exec(*this);// exec must be const
   }

   template <typename Tinput, typename Toutput>
   void operator = (OPConv<Tinput,Toutput> const& op) 
   {
     op.exec(*this);// exec must be const
   }
   //copy constructor *** ATTENTION **** BUG POSSIBLE **********
   //02/05/09
   AlignedArray(AlignedArray const& SigSrc){
     buf            = SigSrc.buf; 
     bufferA        = SigSrc.bufferA;
     UsefulDataSize = SigSrc.UsefulDataSize;
#ifdef SigName
     name           = string("ref_")+SigSrc.name;
#endif
     buf->nbref++;
   }; 
   //Creation of Subsig, this argument can be const& 
   AlignedArray operator () (int start,int stop) const{ 
     AlignedArray tmp; tmp.reference(*this);

     tmp.bufferA       += start;
     tmp.UsefulDataSize = stop-start+1;
#ifdef SigName
     tmp.name           = string("s")+tmp.name;//naming as sref
#endif
     return tmp;
   };
   
   //-------------INIT SIG----------------------------------------------------------------------------------//
   // Set the number of valid entries. New entries will be zero
   void SetSize(int num){setsize(num);};
   void setsize(int num);
   void ones (int num){ this->SetSize(num);  *this = 1;/*do not use memset here*/}
   void zeros(int num){ this->SetSize(num);  memset(bufferA, 0, num*sizeof(TX));/**this = 0;*/}
   void random(int num){ randomInit(*this,num);/*defined in sig_random.hh*/ };
   void wgn(int num);
   //-------------------------------------------------------------------------------------------------------//

  TX   * getptr() const{ return bufferA; };


private:
  buffer<TX>* buf;
  //AlignedArray(AlignedArray const&){};        // Make private copy constructor to prevent copying
  //void operator = (AlignedArray const&){};    // Make private assignment operator to prevent copying
protected:
  int UsefulDataSize;                           // Number of objects stored
  void Error(int e, int n) const;                     // Make fatal error message

};

//MUST INCLUDE .cc TO DEFINE TEMPLATE
#include "AlignedArray.cc"


#endif
