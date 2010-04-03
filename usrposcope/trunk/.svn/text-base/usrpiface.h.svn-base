

#include <QThread>
#include <QImage>
#include <QPainter>
#include <usrp_standard.h>

#include <fftw3.h>

#include "scope.h"

#define STATE_NORMAL 0x0
#define STATE_PAUSED 0x1

#define TRIGGER_I  0x1
#define TRIGGER_Q  0x2
#define TRIGGER_IQ 0x4


class USRPiface : public QThread
{
  Q_OBJECT;

 public:
  USRPiface(Scope *s);
  
 signals:
  void drawingReady();

 public slots:
  void setFreq(int f);
  void setDecim(int val);
  void setTrim(int t);
  void showInph(int state);
  void showQuad(int state);
  void showMixed(int state);
  void showSpec(int state);
  void showFreqMod(int state);
  void showAntiAlias(int state);
  void pauseResume(bool pause);
  void normalize();
  void setTriggerI(bool set);
  void setTriggerQ(bool set);
  void setTriggerIQ(bool set);
  void setTriggerNone(bool set);
  
 protected:
  void run();
  
 private:
  Scope *scope;
  QImage *image;
  usrp_standard_rx *rx;
  bool showInphase;
  bool showQuadrature;
  bool showIQ;
  bool showFFT;
  bool showFM;
  bool antiAlias;
  bool drawPause;
  int trim;
  double timescale;
  int state;
  double max;
  int increment;
  int trigger;
  void drawSignal(short int *data, QPainter *p);
  void drawSignal(int *data, QPainter *p);
  void drawSignal(fftw_complex *data, QPainter *p);
  void setTimescale();
};
