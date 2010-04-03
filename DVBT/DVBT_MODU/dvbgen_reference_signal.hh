#ifndef dvbgen_reference_signal_hh
#define dvbgen_reference_signal_hh
#include "libsig.hh"

Sigui8 get_cp_mask();

Sigui8 get_prbs();

Sigui8 get_tps_mask();
void   get_tps_bits(int ifr , const Sigui8& Sigtps );

float  get_norm_fact();
#endif
