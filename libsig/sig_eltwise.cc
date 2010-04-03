#include "sig_eltwise.hh"

OP<Cxs,__v2Cxs> operator * (const Sigcxs &Sin, const float &alpha)
{
  OP<Cxs,__v2Cxs> op(&mul_scalar_sigcxs_float,Sin,Cxs(alpha));
  return op;
}

void mul_scalar_sigcxs_float(Sigcxs &Sc, const Sigcxs &Sa, const Cxs &Scalar){
  if(Sc.GetSize()==0){
    Sc.SetSize(Sa.GetSize()); 
  }
  float alpha = Scalar.Re(); 
  Sigfl Sc2fl = Sigfl(Sc);

  Sc2fl = Sigfl(Sa) * alpha;
}
