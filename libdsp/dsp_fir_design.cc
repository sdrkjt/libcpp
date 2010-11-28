#include "dsp_fir_design.hh"

#define IzeroEPSILON 1E-21               /* Max error acceptable in Izero */

static double Izero(double x)
{
  double sum, u, halfx, temp;
  int n;

  sum = u = n = 1;
  halfx = x/2.0;
  do {
    temp = halfx/(double)n;
    n += 1;
    temp *= temp;
    u *= temp;
    sum += u;
  } while (u >= IzeroEPSILON*sum);
  return(sum);
}

Sigfl dsp_fir::window (win_type type, int ntaps, double beta)
{
  Sigfl taps; taps.setsize(ntaps);
  int	M = ntaps - 1;	

  switch (type){
  case WIN_RECTANGULAR:
    for (int n = 0; n < ntaps; n++)
      taps[n] = 1;

  case WIN_HAMMING:
    for (int n = 0; n < ntaps; n++)
      taps[n] = 0.54 - 0.46 * cos ((2 * M_PI * n) / M);
    break;

  case WIN_HANN:
    for (int n = 0; n < ntaps; n++)
      taps[n] = 0.5 - 0.5 * cos ((2 * M_PI * n) / M);
    break;

  case WIN_BLACKMAN:
    for (int n = 0; n < ntaps; n++)
      taps[n] = 0.42 - 0.50 * cos ((2*M_PI * n) / (M-1)) - 0.08 * cos ((4*M_PI * n) / (M-1));
    break;

  case WIN_BLACKMAN_hARRIS:
    for (int n = -ntaps/2; n < ntaps/2; n++)
      taps[n+ntaps/2] = 0.35875 + 0.48829*cos((2*M_PI * n) / (float)M) + 
	0.14128*cos((4*M_PI * n) / (float)M) + 0.01168*cos((6*M_PI * n) / (float)M);
    break;

  case WIN_KAISER:
    {
      double IBeta = 1.0/Izero(beta);
      double inm1 = 1.0/((double)(ntaps));
      double temp;
      //fprintf(stderr, "IBeta = %g; inm1 = %g\n", IBeta, inm1);

      for (int i=0; i<ntaps; i++) {
	temp = i * inm1;
	//fprintf(stderr, "temp = %g\n", temp);
	taps[i] = Izero(beta*sqrt(1.0-temp*temp)) * IBeta;
	//fprintf(stderr, "taps[%d] = %g\n", i, taps[i]);
      }
    }
    break;

  default:
    MSG_ERROR("dsp_fir:window: type unknown");
  }

  return taps;
}

// width_factor/delta_f gives number of taps required.
static const float width_factor[5] = {   // indexed by win_type
  3.3,			// WIN_HAMMING
  3.1,			// WIN_HANN
  5.5,              	// WIN_BLACKMAN
  2.0,                  // WIN_RECTANGULAR
  //5.0                   // WIN_KAISER  (guesstimate compromise)
  //2.0                   // WIN_KAISER  (guesstimate compromise)
  10.0			// WIN_KAISER
};

int dsp_fir::compute_ntaps (double sampling_freq,
			    double transition_width,
			    win_type window_type,
			    double beta)
{
  // normalized transition width
  double delta_f = transition_width / sampling_freq;

  // compute number of taps required for given transition width
  int ntaps = (int) (width_factor[window_type] / delta_f + 0.5);

  ntaps += 1-(ntaps & 1);	// if even,make odd

  return ntaps;
}

Sigfl dsp_fir::low_pass (double gain,
			 double sampling_freq,
			 double cutoff_freq,    	// Hz center of transition band
			 double transition_width,	// Hz width of transition band
			 win_type window_type,
			 double beta)		// used only with Kaiser
{
  int ntaps = compute_ntaps (sampling_freq, transition_width,
			     window_type, beta);

  // construct the truncated ideal impulse response
  // [sin(x)/x for the low pass case]
  
  Sigfl taps; taps.setsize(ntaps);
  Sigfl w = window (window_type, ntaps, beta);

  int M = (ntaps - 1) / 2;
  double fwT0 = 2 * M_PI * cutoff_freq / sampling_freq;

  for (int n = -M; n <= M; n++){
    if (n == 0)
      taps[n + M] = fwT0 / M_PI * w[n + M];
    else {
      // a little algebra gets this into the more familiar sin(x)/x form
      taps[n + M] =  sin (n * fwT0) / (n * M_PI) * w[n + M];
    }
  }
  
  // find the factor to normalize the gain, fmax.
  // For low-pass, gain @ zero freq = 1.0
  
  double fmax = taps[0 + M];
  for (int n = 1; n <= M; n++)
    fmax += 2 * taps[n + M];

  gain /= fmax;	// normalize

  for (int i = 0; i < ntaps; i++)
    taps[i] *= gain;

  return taps;
}
