

#ifndef __SCOPE_H
#define __SCOPE_H

#include <QWidget>
#include <QPainter>
#include <QImage>


class Scope : public QWidget
{
  Q_OBJECT;

 public:
  Scope();
  QImage *image;
  bool ready;

 public slots:
  void saveScreen();
  void updateScope();
  void setTimeDiv(double s);

 protected:
  void paintEvent(QPaintEvent *e = NULL);

 private:
  uchar *imagedata;
  double timeDiv;
};


#endif
