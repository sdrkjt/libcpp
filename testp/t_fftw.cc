#include <fftw3.h>
#include "TSC.hh"
#include "libsig.hh"


int test_fft(int argc, char **argv)
{

  fftwf_complex *input, *out;
  fftwf_plan p;
  int N = 8192;
  Sigcxs Sin; Sin.SetSize(N);

  input  = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);
  out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);


  TIME();
  p = fftwf_plan_dft_1d(N, input, out, FFTW_FORWARD,  FFTW_MEASURE);
       /*FFTW_ESTIMATE  FFTW_MEASURE FFTW_PATIENT */
  TIME();

  for(int ii=0;ii<N;ii++){
    input[ii][0] = 0;
    input[ii][1] = 0;
    Sin[ii] = Cxs(0.0f,0.0f);
  }
  input[2][0] = 1.0f; 
  Sin[2] = Cxs(1.0f,0.0f);

  for(int ii=0;ii<10;ii++)
      cout<<"("<<input[ii][0]<<","<<input[ii][1]<<")"<<endl;


  for(int ii=0;ii<10;ii++){     
    TIME();
    fftwf_execute(p); /* repeat as needed */
    TIME();
  }
 
  for(int ii=0;ii<10;ii++)
    cout<<"("<<input[ii][0]<<","<<input[ii][1]<<")"<<endl;

  cout<<"----First 10---------\n";
  for(int ii=0;ii<10;ii++)
    cout<<"("<<out[ii][0]<<","<<out[ii][1]<<")"<<endl;
  cout<<"----Last  10---------\n";
  for(int ii=N-10;ii<N;ii++)
    cout<<"("<<out[ii][0]<<","<<out[ii][1]<<")"<<endl;
 
  DUMP(Sin);
  Sigcxs Sout; 
  for(int ii=0;ii<10;ii++){     
    TIME();
    Sout = dft(Sin);
    TIME("dft on Sig");
  }
  DUMP(Sout);

  Sigcxs Sinverse;
  Sinverse = idft(Sout);
  DUMP(Sinverse);

  figure(1);
  plot2d(out,N,"FFT");
  
  int test3 = 11;
  DUMPBIT(int ,test3);
  int test4 = test3>>3;
  DUMPBIT(int ,test4);
short int state = 0x9500;
 DUMPBIT(short int, state);
  fftwf_destroy_plan(p);
  fftwf_free(input); fftwf_free(out);
  return -1;
}
int main(int argc, char **argv)
{
  QApplication a(argc, argv);
  SdrGui *  pGui1 = new SdrGui();
 

  SdrThread *plot1 = new SdrThread(pGui1,&test_fft);
  plot1->start();

  return a.exec();
}
