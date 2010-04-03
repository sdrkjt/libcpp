#ifndef sig_dump_hh
#define sig_dump_hh

#include "AlignedArray.hh"
#include "sig_types.hh"

#define DefOpConv(funId, funExeId,methId,Tin,Tout)		\
  static inline void funExeId(Tout& Sout, const Tin & Sin)	\
  {								\
    int sizeIn  = Sin .GetSize();				\
    int sizeOut = Sout.GetSize();				\
    if(sizeOut == 0)						\
      {								\
	Sout.SetSize(sizeIn);					\
	sizeOut = sizeIn;					\
      }								\
    if(sizeOut!=sizeIn)						\
      {								\
	cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	\
	myabort();						\
      };							\
    for(int ii=0;ii<sizeOut;ii++)				\
      Sout[ii] = (Sin[ii]). methId ();			\
  }								\
  static inline OPConv<Tin,Tout> funId (const Tin & Sin)	\
  {								\
    OPConv<Tin,Tout> op (&funExeId,Sin);			\
    return op;							\
  }								\
 

DefOpConv(real,realExec,Re,Sigcxs,Sigfl);// x = real(y);
DefOpConv(imag,imagExec,Im,Sigcxs,Sigfl);// x = imag(y) 
DefOpConv(real,realExec,Re,Sigcxd,Sigdb);
DefOpConv(imag,imagExec,Im,Sigcxd,Sigdb);

#endif
