#ifndef sdrgui_hh
#define sdrgui_hh

#include "plot.hh"
void figure(int figId, const QString & title ="notitle");
void curve(int curveId);
void setpen(const QString &colStr,const QString &symStr,const QString &stySt);
void plot(const SdrData& data, const QString & title ="notitle");


#endif
