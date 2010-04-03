#include "libsig.hh"
#define nbLu 14
class module{
public:
  int size;
  Sigfl Sa,Sb;//,Sas,Sbs;
  Sigfl Sc[nbLu];

  void init(int size);
  void compute();

};
