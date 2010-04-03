#include "sig_dft.hh"

void dftExec(Sigcxs& Sout, const Sigcxs & Sin)	 
{								 
  int sizeIn  = Sin .GetSize();				 
  int sizeOut = Sout.GetSize();				 
  if(sizeOut == 0)						 
    {								 
      Sout.SetSize(sizeIn);					 
      sizeOut = sizeIn;					 
    }								 
  if(sizeOut!=sizeIn)						 
    {								 
      cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	 
      myabort();						 
    };	

  fftwplan<fftwf_complex,fftwf_plan> plan(sizeIn);

  memcpy((char*)(plan.input),(char*)(Sin.bufferA),sizeIn*sizeof(fftwf_complex));
  fftwf_execute(plan.planForw);
  memcpy((char*)(Sout.bufferA),(char*)(plan.output),sizeIn*sizeof(fftwf_complex));
}								 
OPConv<Sigcxs,Sigcxs> dft(const Sigcxs & Sin)	 
{								 
  OPConv<Sigcxs,Sigcxs> op (&dftExec,Sin);			 
  return op;							 
}								 

void idftExec(Sigcxs& Sout, const Sigcxs & Sin)	 
{								 
  int sizeIn  = Sin .GetSize();				 
  int sizeOut = Sout.GetSize();				 
  if(sizeOut == 0)						 
    {								 
      Sout.SetSize(sizeIn);					 
      sizeOut = sizeIn;					 
    }								 
  if(sizeOut!=sizeIn)						 
    {								 
      cout<<c_red<<"Error: Size mismatched"<<c_normal<<endl;	 
      myabort();						 
    };	

  fftwplan<fftwf_complex,fftwf_plan> plan(sizeIn);

  memcpy((char*)(plan.input),(char*)(Sin.bufferA),sizeIn*sizeof(fftwf_complex));
  fftwf_execute(plan.planBckw);
  memcpy((char*)(Sout.bufferA),(char*)(plan.output),sizeIn*sizeof(fftwf_complex));
  //Normalisation
  Sout = Sout * (1.f/sizeIn);
}								 
OPConv<Sigcxs,Sigcxs> idft(const Sigcxs & Sin)	 
{								 
  OPConv<Sigcxs,Sigcxs> op (&idftExec,Sin);			 
  return op;							 
}
