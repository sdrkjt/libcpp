#include "sig_gui.hh"

void plot2d(const Sigcxs& input,const QString& title)
{
  SdrSigCx<Sigcxs> pl_input (input);
  plot(pl_input,title);
};

void plot2d(fftwf_complex* input,const int& Size,const QString& title)
{
  SdrFFT pl_input (input,Size);
  plot(pl_input,title);
};

