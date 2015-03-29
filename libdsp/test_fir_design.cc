#include "sig_gui.hh"
#include "dsp_fir_design.hh"
#include "dsp_resampler.hh"

#include "dsp_filter.hh"

int test_fir_design (int argc, char **argv){
  double fractional_bw = 0.4;
  double interpolation = 7;
  double decimation    = 8;
  
  double beta = 5.0;
  double trans_width = 0.5 - fractional_bw;
  double mid_transition_band = 0.5 - trans_width/2;

  Sigfl taps; taps = dsp_fir::low_pass(interpolation,                     // gain
				       1,                                 // Fs
				       mid_transition_band/interpolation, // trans mid point
				       trans_width/interpolation,         // transition width
				       dsp_fir::WIN_KAISER,
				       beta                               // beta
				       );
  DUMP(taps);

  figure(1); curve(1); plot1d(taps,"taps");mygetchar();

  int SIZE  = 2048-707;

  Sigcxs I_dat; I_dat.random(SIZE);
  DUMP(I_dat);
 
  dsp_filter filter(taps,SIZE);

  Sigcxs O_dat;
  TIME();
  filter.fft_compute(O_dat,I_dat);
  TIME("fft_compute");
  DUMP(O_dat);

  filter.restart();
  TIME();
  filter.sse_compute(O_dat,I_dat);
  TIME("sse_compute");
  DUMP(O_dat);
  figure(2); curve(1); plot1d(real(O_dat),"Output1");

  filter.restart();
  TIME();
  filter.cpp_compute(O_dat,I_dat);
  TIME("cpp_compute");

  DUMP(O_dat); 
  figure(2); curve(2); plot1d(real(O_dat),"Output2");



  rational_resampler u_resampler(interpolation,decimation,fractional_bw);mygetchar();

}

int main (int argc, char **argv)
{
  QApplication a(argc, argv);
  SdrGui   pGui;

  int (*worker) (int, char **);
  worker = test_fir_design;
  SdrThread plot(&pGui,worker);
  plot.start();

  return a.exec();
}
