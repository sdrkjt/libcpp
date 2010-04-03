
using namespace std;
#include <iostream>
#include <QCheckBox>
#include <QMutex>
#include <QWaitCondition>

#include <math.h>
#include <unistd.h>
#include <usrp_standard.h>


#include "usrpiface.h"

#define BUFSIZE 2 * 65536
#define DIVSIZE 50

extern QMutex mutex;
extern QWaitCondition waitCond;

static void
print_stats(usrp_standard_rx_sptr r)
{
  cout << "Serial Number: " << r->serial_number() << endl;
  cout << "Converter Rate: " << r->converter_rate() << endl;
  cout << "RX Frequency: " << r->rx_freq(0) << endl;
  cout << "Channels: " << r->nchannels() << endl;
  cout << "Decimation Rate: " << r->decim_rate() << endl;
  cout << "Mux: " << hex << r->mux() << endl;
  cout << "Format: " << hex << r->format() << dec << endl;
  cout << "Block Size: " << r->block_size() << endl;
  cout << "Daughterboard: " << r->daughterboard_id(0) << endl;
  cout << "Daughterboard: " << r->daughterboard_id(1) << endl;
}


USRPiface::USRPiface(Scope *s)
{
  scope = s;

  rx = usrp_standard_rx::make(0, 256);
  setTrim(2);
  setDecim(256);
  setFreq(1040);
  rx->set_pga(0, rx->pga_max());
  print_stats(rx);


  state = STATE_NORMAL;
  max = 0.1;
  showInphase = true;
  showQuadrature = true;
  showIQ = false;
  showFFT = false;
  showFM = false;
  antiAlias = true;
  trigger = TRIGGER_I;
  drawPause = false;
}


void
USRPiface::drawSignal(short int *data, QPainter *p)
{
  int swidth = scope->width();
  int oldval = 200;
  double newval = 0;
  for (int i = 0; i <= swidth; i++)
    {
      if (*data > max)
	{
	  max = *data;
	}
      newval = 200 - (200 * (*data) / max);
      p->drawLine(i, oldval, i + 1, (int) newval);
      oldval = (int) newval;
      data += increment;
    }
}


void
USRPiface::drawSignal(int *data, QPainter *p)
{
  int swidth = scope->width();
  int oldval = 200;
  double newval = 0;
  for (int i = 0; i <= swidth; i++)
    {
      newval = 200 - (200 * (*data) / max);
      p->drawLine(i, oldval, i + 1, (int) newval);
      oldval = (int) newval;
      data++;
    }
}

void
USRPiface::drawSignal(fftw_complex *data, QPainter *p)
{
  int swidth = scope->width();
  double maxh = 1.0;
  double gdata[swidth];
  for (int i = 0; i <= swidth; i++)
    {
      double *val = (double *) &data[i];
      gdata[i] = log(pow(val[0], 2) + pow(val[1], 2));
      if (gdata[i] > maxh)
	{
	  maxh = gdata[i];
	}
      data++;
    }
  int oldval = 400;
  for (int i = 1; i < swidth; i++)
    {
      int index =  (i + (swidth / 2)) % swidth;
      int newval = (int) (400 - 350 * gdata[index] / maxh);
      p->drawLine(i - 1, oldval, i, newval);
      oldval = newval;
    }
}


void
USRPiface::run()
{
  char buf[BUFSIZE];
  char complexbuf[BUFSIZE];
  bool overrun = false;

  for (unsigned int i = 0; i < BUFSIZE; i++)
    {
      buf[i] = 0;
    }

  QRectF bgrect(0, 0, scope->width(), scope->height());


  rx->start();
  while (1)
    {
      int swidth = scope->width();
      int sheight = scope->height();
      increment = trim;  

      if (state != STATE_NORMAL)
	{
	  if (drawPause)
	    {
	      drawPause = false;
	      mutex.lock();
	      emit(drawingReady());
	      waitCond.wait(&mutex);	
	      mutex.unlock();
	    }
	  usleep(200);
	  continue;
	}

      //read data from USRP
      overrun = true;
      while(overrun)
	{
	  int bytesread = rx->read(buf, BUFSIZE, &overrun);
	  if (bytesread < BUFSIZE)
	    { 
	      cout << "few bytes: " << bytesread << endl; 
	    }
	}

      QPainter p(scope->image);
      //draw background
      p.fillRect(bgrect, QColor(80,80,80));
      p.setPen(QColor(95,95,95)); 
      for (int i = DIVSIZE; i < swidth; i += DIVSIZE)
	{
	  p.drawLine(i, 0, i, sheight);
	  p.drawLine(0, i, swidth, i);
	}

      if (antiAlias)
	{
	  p.setRenderHint(QPainter::Antialiasing, true);
	}
      else
	{
	  p.setRenderHint(QPainter::Antialiasing, false);
	}


      //Do triggering
      short int *data = (short int *) &buf;
      void *trigdata = (void *) data;
      int *complexdata = (int *) &complexbuf;
      int *complextrig = complexdata;
      if ((trigger & TRIGGER_I) || (trigger & TRIGGER_Q))
	{
	  if (trigger & TRIGGER_Q)
	    data++;
	  trigdata = (void *) data;
	  short int lmax = *data;
	  for (int i = 0; i <= swidth * 2; i++)
	    {
	      if (*data > lmax && i < swidth)
		{
		  trigdata = (void *) data;
		  lmax = *data;
		}
	      data += increment;
	    }
	}

      //Make sure we're pointing to I and not Q data
      trigdata = (void *) (((long int) trigdata) & (~0x2));

      //Complex data calculation, and triggering if triggered on IQ channel
      if ((trigger & TRIGGER_IQ) || showIQ)
	{
	  data = (short int *) (((long int) trigdata) & (~0x1)); // make sure to start on an I value
	  int lmax = 0;
	  for (int i = 0; i <= swidth * 2; i++)
	    {
	      int h = (int) sqrt(pow(data[0], 2) + pow(data[1], 2));
	      complexdata[i] = h;

	      if (h > max) 
		{
		  max = h;
		}

	      if (h > lmax && i < swidth && trigger & TRIGGER_IQ)
		{
		  lmax = h;
		  trigdata = (void *) data; 
		  complextrig = complexdata + i;
		}
	      data += increment;
	    }
	}

     
      //FM demodulation
      if (showFM)
	{
	  p.setPen(QColor(10,10,250));
	  data = (short int *) (((long int) trigdata) & (~0x1)); // make sure to start on an I value
	  double dt = 64000000 / (trim * rx->decim_rate());
	  double oldI = 1, oldQ = 1;
	  double oldinstfreq = 0;
	  double old2 = 0;
	  double maxNorm = 0;
	  for (int i = 0; i < swidth * 2; i++)
	    {
	      //differentiate arctan(I/Q) (i.e. phase) to get instantaneous frequency
	      double newI = (double) data[0];
	      double newQ = (double) data[1];
	     
	      //normalize amplitude
	      double norm = sqrt(pow(newI, 2) + pow(newQ, 2));
	      newI /= norm;
	      newQ /= norm;
	      if (norm > maxNorm)
		{
		  maxNorm = norm;
		}

	      double instantfreq = (newI * ((newQ - oldQ) / dt) - newQ * ((newI - oldI) / dt));

	      oldI = newI;
	      oldQ = newQ;
	      data += trim;
	      p.drawLine(i, (int) (200 + (1000 * maxNorm * oldinstfreq)), i + 1, (int) (200 + (1000 * maxNorm * instantfreq)));
	      old2 = oldinstfreq;
	      oldinstfreq = instantfreq;
	    }
	}


      //fft 
      if (showFFT)
	{
	  fftw_complex *in, *out;
	  fftw_plan plan;
	 
	  int N = swidth * 2;
	 
	  in = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * N);
	  out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * N);
	  plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	  data = (short int *) buf;
	  for (int i = 0; i < N; i++)
	    {
	      double *num = (double *) &in[i];
	      num[0] = (double) data[0];
	      num[1] = (double) data[1];
	      data += 2; 
	    }

	  fftw_execute(plan);

	  p.setPen(QColor(250,250,200));
	  drawSignal(out, &p);

	  fftw_destroy_plan(plan);
	  fftw_free(in);
	  fftw_free(out);
	}



      //I signal
      if (showInphase)
	{
	  p.setPen(QColor(100,150,200));
	  drawSignal((short int *) trigdata, &p);
	}

      //Q signal
      data++;
      if (showQuadrature)
	{
	  p.setPen(QColor(250,250,100));
	  drawSignal(((short int *) trigdata) + 1, &p);
	}

      //IQ signal
      if (showIQ)
	{
	  p.setPen(QColor(150,250,150));
	  drawSignal(complextrig, &p);
	}
	 
      p.end();

      if (state == STATE_NORMAL)
	{
          mutex.lock();
          emit(drawingReady());
          waitCond.wait(&mutex);	
	  mutex.unlock();
	}

    }
  rx->stop();
}
  

void
USRPiface::setFreq(int f)
{
  double freq = (double) f;
  rx->set_rx_freq(0, freq * 1000);
  cout << "Set freq to " << (int) rx->rx_freq(0) << endl;
}

void 
USRPiface::setTimescale()
{
  int dec = rx->decim_rate() * trim;
  timescale = 1000000 * ((double) (dec * DIVSIZE)) / ((double) 64000000.0); //microseconds per DIV
  cout << "New timescale: " << timescale << "uSec/Div" << endl;
  scope->setTimeDiv(timescale); //microseconds
}

void
USRPiface::setDecim(int val)
{
  rx->set_decim_rate(val);
  timescale = 1000 * (double) (scope->width() * val) / (double) 64000000.0; //milliseconds
  cout << "Set decimation rate to: " << rx->decim_rate() << endl;
  setTimescale();
}

void
USRPiface::setTrim(int t)
{
  trim = t;
  cout << "Set trim rate to " << trim << endl;
  setTimescale();
}


void
USRPiface::pauseResume(bool pause)
{
  if (pause)
    {
      state = STATE_PAUSED;
      drawPause = true;
    }
  else
    state = STATE_NORMAL;
}


void
USRPiface::normalize()
{
  max = 0;
}

void
USRPiface::showQuad(int state)
{
  if (state == Qt::Checked)
    showQuadrature = true;
  else
    showQuadrature = false;
}

void
USRPiface::showAntiAlias(int state)
{
  if (state == Qt::Checked)
    antiAlias = true;
  else
    antiAlias = false;
}

void
USRPiface::showInph(int state)
{
  if (state == Qt::Checked)
    showInphase = true;
  else
    showInphase = false;
}

void
USRPiface::showMixed(int state)
{
  if (state == Qt::Checked)
    showIQ = true;
  else
    showIQ = false;
}

void
USRPiface::showSpec(int state)
{
  if (state == Qt::Checked)
    showFFT = true;
  else
    showFFT = false;
}

void
USRPiface::showFreqMod(int state)
{
  if (state == Qt::Checked)
    showFM = true;
  else
    showFM = false;
}

void 
USRPiface::setTriggerI(bool set)
{
  if (set)
    {
      trigger = TRIGGER_I;
      cout << "Trigger I" << endl;
    }
}

void 
USRPiface::setTriggerQ(bool set)
{
  if (set)
    {
      trigger = TRIGGER_Q;
      cout << "Trigger Q" << endl;
    }
}

void 
USRPiface::setTriggerIQ(bool set)
{
  if (set)
    {
      trigger = TRIGGER_IQ;
      cout << "Trigger IQ" << endl;
    }
}

void 
USRPiface::setTriggerNone(bool set)
{
  if (set)
    {
      trigger = 0;
      cout << "No Trigger" << endl;
    }
}
