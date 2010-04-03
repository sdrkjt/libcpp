#ifndef sig_gui_hh
#define sig_gui_hh


#include "sdrgui.hh"
#include "sig_types.hh"

template <typename TS>	
class SdrSigRe: public SdrData
{

public:
  SdrSigRe(const TS& input): SdrData()
  {
    data.SetSize(input.GetSize());
    data  = input;
  }

  virtual SdrData *copy() const
  {
    //printf("copy QwtData\n");
    return new SdrSigRe(data);
  }

  size_t size() const
  {
    return data.GetSize();
  }

  double x(size_t i) const
  {
    return i;
  }

  double y(size_t i) const
  {  
    return data[i];
  }

private:
  TS     data;
};
template <typename TS>	
class SdrSigCx: public SdrData
{

public:
  SdrSigCx(const TS& input): SdrData()
  {
    data.SetSize(input.GetSize());
    data  = input;
  }

  virtual SdrData *copy() const
  {
    //printf("copy QwtData\n");
    return new SdrSigCx(data);
  }

  size_t size() const
  {
    return data.GetSize();
  }

  double x(size_t i) const
  {
    return (data[i]).Re();
  }

  double y(size_t i) const
  {  
    return (data[i]).Im();
  }

private:
  TS     data;
};

#include <fftw3.h>
class SdrFFT: public SdrData
{

public:
  SdrFFT(fftwf_complex* input, const int& Size): SdrData()
  {
    d_size = Size;
    data = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * Size);
    for(int ii=0;ii<Size;ii++){
      data[ii][0]  = input[ii][0];
      data[ii][1]  = input[ii][1];
    }
  }

  virtual SdrData *copy() const
  {
    //printf("copy QwtData\n");
    return new SdrFFT(data,d_size);
  }

  size_t size() const
  {
    return d_size;
  }

  double x(size_t i) const
  {
    return data[i][0];
  }

  double y(size_t i) const
  {  
    return data[i][1];
  }

private:
  size_t d_size;
  fftwf_complex*     data;
};

template <typename SX>
void plot1d(const SX &input, const QString& title = "notitle")
{
  SdrSigRe<SX> pl_input (input);
  plot(pl_input,title);
};


void plot2d(const Sigcxs& input, const QString& title = "notitle");
void plot2d(fftwf_complex* input, const int& Size, const QString& title = "notitle");


#endif 
