

using namespace std;

#include "scope.h"
#include <iostream>
#include <QMutex>
#include <QWaitCondition>

QMutex mutex;
QWaitCondition waitCond;

Scope::Scope()
{
  int width = 800;
  int height = 400;
  imagedata = new uchar[sizeof(int) * width * height];
  image = new QImage(imagedata, width, height, QImage::Format_ARGB32);
  timeDiv = 0;
  setMinimumSize(width, height);
  ready = true;
}

void
Scope::saveScreen()
{
  QPixmap pm = QPixmap::grabWidget(this);
  pm.save("oscopeShot.png", "PNG");
}

void
Scope::setTimeDiv(double s)
{
  timeDiv = s;
}

void
Scope::updateScope()
{
  update();
}

void
Scope::paintEvent(QPaintEvent *e)
{ 
  mutex.lock();
  QPainter p(this);
  p.drawImage(0, 0, *image);
  p.setPen(QColor(250, 250, 250, 100));
  p.drawText(20, 385, QString().setNum(timeDiv / 2) + tr(" uSec/Div"));
  p.end();
  ready = true;
  waitCond.wakeAll();
  mutex.unlock();
}

