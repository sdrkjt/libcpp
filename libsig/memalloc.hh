#ifndef memalloc_hh
#define memalloc_hh

#include <cstring>   
#include "cpp_monit.hh"
#include "sig_macros.hh"
#include <vector>

#ifdef MEMALLOC
#define MAX_MEM_SIZE 300000 //1MB
#else 
#define MAX_MEM_SIZE 1
#endif

#define memfree(x)  {memfreeok(x);/*mytrace();*/}

#ifdef W_MEMALLOC
#define WarningAlloc(){ MSG_WARNING("memalloc is used!!"); }
#else 
#define WarningAlloc()
#endif

typedef   signed long long   int64; 

struct ida{
  int64 adr;
  int64 size;
  int64 birth;
  int64 death;
};

class sta{
public:
  static char* MEM; //[MAX_MEM_SIZE];

  //static char MEM[MAX_MEM_SIZE];
  static int64  pAdr;
  static int64  clock;

  static vector<ida*>lmember;

  static inline int64 myalloc(const int64 &size){
    ida* newmember = new ida();
    newmember->adr   = pAdr;
    newmember->size  = size;
    newmember->birth = clock;
    newmember->death = -1;

    pAdr  += size;//(size-(size&15))+(((size&15)>0)?16:0); /*simple version which is not efficient*/
                  /*alignment is currently managed by AligneArray,todo: eliminate scalar alloc*/
    if(pAdr>MAX_MEM_SIZE-1){
      MSG_ERROR("Bad Allocation, maxsize excessed: pAdr="<<pAdr<<">"<<MAX_MEM_SIZE);
    }
    clock += 1; /*counter of evenement*/
    lmember.push_back(newmember);

    return (int64)(MEM+newmember->adr);
  };

  static int64 myfree(const int64 &atAdr){
    int64 rAdr = atAdr - (int64)MEM;
    int64 existmember = 0;
    FORLIST(lmember,ii){
      if( (lmember[ii]->adr == rAdr)&&(lmember[ii]->death == -1) ){
	lmember[ii]->death = clock;
	existmember = 1;
      } 
    }
    if(existmember==0){
      dumpMemberList();
      DUMP(rAdr);DUMP((int64)MEM);DUMP(atAdr);
      MSG_ERROR("free memory uncorrectly!!");
    }
    clock += 1; /*counter of evenement*/
    return -1;
  };

  static void dumpMemberList(){
    FORLIST(lmember,ii){
      DUMP(ii);
      DUMP(lmember[ii]->  adr);
      DUMP(lmember[ii]-> size);
      DUMP(lmember[ii]->birth);
      DUMP(lmember[ii]->death);
    }
  }
  static void init(){
    memset(MEM,0,MAX_MEM_SIZE);
  }
};

/*1D Alloc*/
template<typename TX>
inline TX* memalloc(int64 size){
  TX* Adr;
#ifndef MEMALLOC
  Adr = new TX[size];
#else
  Adr = (TX*)sta::myalloc(size*sizeof(TX));
  WarningAlloc();
#endif
  return Adr;
};

template<typename TX>
inline void memfreeok(TX* &ptab){
#ifndef MEMALLOC
  delete[] ptab;
#else
  sta::myfree((int64)ptab);
#endif
};

/*2D Alloc*/
template<typename TX>
inline TX** memalloc(int64 nbrow, int64 nbcol){
  TX** ptab   = new TX* [nbrow];  
  TX*  tmpAdr = memalloc<TX>(nbrow*nbcol);
  for(int64 ii=0;ii<nbrow;ii++){
    ptab[ii] = tmpAdr+ii*nbcol;
  }
  return ptab;
};

template<typename TX>
inline void memfreeok(TX** &ptab){
  memfreeok<TX>(ptab[0]);
  delete[] ptab;
};



#endif //memalloc_hh
