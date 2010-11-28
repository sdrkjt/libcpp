#ifndef dsp_fir_design
#define dsp_fir_design

#include "libsig.hh"

class dsp_fir {
public:

  enum win_type {
    WIN_HAMMING = 0,	// max attenuation 53 dB
    WIN_HANN = 1,	// max attenuation 44 dB
    WIN_BLACKMAN = 2,	// max attenuation 74 dB
    WIN_RECTANGULAR = 3,
    WIN_KAISER = 4,     // max attenuation a function of beta, google it
    WIN_BLACKMAN_hARRIS = 5,
  };

  static Sigfl window (win_type type, int ntaps, double beta);

  static int compute_ntaps (double sampling_freq,
			    double transition_width,
			    win_type window_type,
			    double beta);

  static Sigfl low_pass (double gain,
			 double sampling_freq,
			 double cutoff_freq,	// Hz center of transition band
			 double transition_width,	// Hz width of transition band
			 win_type window_type,
			 double beta);		// used only with Kaiser
};

#endif
