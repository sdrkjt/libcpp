

#include <QApplication>
#include <QGridLayout>
#include <QSpinBox>
#include <QLabel>
#include <QMenuBar>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>

#include "scope.h"
#include "usrpiface.h"

int main (int argc, char *argv[])
{
  QApplication *a = new QApplication(argc, argv);
  Scope *s = new Scope();
  USRPiface *rp = new USRPiface(s);
  QGridLayout *grid = new QGridLayout();
  QWidget *central = new QWidget();

  //QMenuBar *menubar = new QMenuBar();
  //grid->addWidget(menubar, 0, 0, 1, 10);
  //QMenu *settings = menubar->addMenu(QObject::tr("Settings"));

  grid->addWidget(s, 1, 0, 1, 10);
  
  QSpinBox *freq = new QSpinBox();
  freq->setRange(0, 60000);
  freq->setValue(1040);
  grid->addWidget(new QLabel(QObject::tr("Frequency:")), 2, 0, 1, 2);
  grid->addWidget(freq, 3, 0, 1, 1);
  grid->addWidget(new QLabel(QObject::tr("(kHz)")), 3, 1, 1, 1);
  QObject::connect(freq, SIGNAL(valueChanged(int)), rp, SLOT(setFreq(int)));

  QSpinBox *decim = new QSpinBox();
  decim->setRange(8, 256);
  decim->setValue(256);
  decim->setSingleStep(2);
  grid->addWidget(new QLabel(QObject::tr("Decimation:")), 2, 2, 1, 1);
  grid->addWidget(decim, 3, 2, 1, 1);
  QObject::connect(decim, SIGNAL(valueChanged(int)), rp, SLOT(setDecim(int)));

  QSpinBox *timescale = new QSpinBox();
  timescale->setRange(2, 16);
  timescale->setValue(2);
  timescale->setSingleStep(2);
  grid->addWidget(new QLabel(QObject::tr("Time Scale:")), 2, 3, 1, 1);
  grid->addWidget(timescale, 3, 3, 1, 1);
  QObject::connect(timescale, SIGNAL(valueChanged(int)), rp, SLOT(setTrim(int)));


  grid->addWidget(new QLabel(QObject::tr("Display")), 2, 5, 1, 1);

  QCheckBox *doAntiAlias = new QCheckBox(QObject::tr("Antialiased Lines"));
  doAntiAlias->setChecked(true);
  grid->addWidget(doAntiAlias, 5, 0, 1, 1);
  QObject::connect(doAntiAlias, SIGNAL(stateChanged(int)), rp, SLOT(showAntiAlias(int)));

  QCheckBox *showInph = new QCheckBox(QObject::tr("I Channel"));
  grid->addWidget(showInph, 3, 5, 1, 1);
  QObject::connect(showInph, SIGNAL(stateChanged(int)), rp, SLOT(showInph(int)));

  QCheckBox *showQuad = new QCheckBox(QObject::tr("Q Channel"));
  grid->addWidget(showQuad, 4, 5, 1, 1);
  QObject::connect(showQuad, SIGNAL(stateChanged(int)), rp, SLOT(showQuad(int)));

  QCheckBox *showAM = new QCheckBox(QObject::tr("AM Demod"));
  grid->addWidget(showAM, 5, 5, 1, 1);
  QObject::connect(showAM, SIGNAL(stateChanged(int)), rp, SLOT(showMixed(int)));

  QCheckBox *showFM = new QCheckBox(QObject::tr("FM Demod"));
  grid->addWidget(showFM, 6, 5, 1, 1);
  QObject::connect(showFM, SIGNAL(stateChanged(int)), rp, SLOT(showFreqMod(int)));

  QCheckBox *showFFT = new QCheckBox(QObject::tr("Spectrum Analyzer (FFT)"));
  grid->addWidget(showFFT, 7, 5, 1, 1);
  QObject::connect(showFFT, SIGNAL(stateChanged(int)), rp, SLOT(showSpec(int)));

  grid->addWidget(new QLabel(QObject::tr("Trigger")), 2, 8, 1, 1, Qt::AlignHCenter);
  QRadioButton *trigI = new QRadioButton(QObject::tr("I Channel"));
  QRadioButton *trigQ = new QRadioButton(QObject::tr("Q Channel"));
  QRadioButton *trigIQ = new QRadioButton(QObject::tr("AM Demod"));
  QRadioButton *trigNone = new QRadioButton(QObject::tr("None"));
  trigI->setChecked(true);
  grid->addWidget(trigI, 3, 8, 1, 1);
  grid->addWidget(trigQ, 4, 8, 1, 1);
  grid->addWidget(trigIQ, 5, 8, 1, 1);
  grid->addWidget(trigNone, 6, 8, 1, 1);
  QObject::connect(trigI, SIGNAL(toggled(bool)), rp, SLOT(setTriggerI(bool)));
  QObject::connect(trigQ, SIGNAL(toggled(bool)), rp, SLOT(setTriggerQ(bool)));
  QObject::connect(trigIQ, SIGNAL(toggled(bool)), rp, SLOT(setTriggerIQ(bool)));
  QObject::connect(trigNone, SIGNAL(toggled(bool)), rp, SLOT(setTriggerNone(bool)));

  QPushButton *runstop = new QPushButton(QObject::tr("Run/Stop"));
  runstop->setCheckable(true);
  QObject::connect(runstop, SIGNAL(toggled(bool)), rp, SLOT(pauseResume(bool)));
  grid->addWidget(runstop, 2, 9, 1, 1);

  QPushButton *save = new QPushButton(QObject::tr("Save Image"));
  QObject::connect(save, SIGNAL(clicked()), s, SLOT(saveScreen()));
  grid->addWidget(save, 3, 9, 1, 1);

  QPushButton *normalize = new QPushButton(QObject::tr("Normalize Y"));
  QObject::connect(normalize, SIGNAL(clicked()), rp, SLOT(normalize()));
  grid->addWidget(normalize, 4, 9, 1, 1);

  central->setLayout(grid);

  QObject::connect(rp, SIGNAL(drawingReady()), s, SLOT(updateScope()));

  central->resize(810,610);  central->show();

  rp->start();
  return a->exec();
}
