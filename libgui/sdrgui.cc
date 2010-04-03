#include "sdrgui.hh"
void figure(int figId,const QString & title)
{
  SdrThread* pcur =  (SdrThread*)QThread::currentThread();
  pcur->figure(figId,title);
}
void setpen(const QString &colStr,const QString &symStr,const QString &stySt){
  SdrThread* pcur =  (SdrThread*)QThread::currentThread();
  pcur->setpen(colStr,symStr,stySt);
}
void curve(int curveId)
{
  SdrThread* pcur =  (SdrThread*)QThread::currentThread();
  pcur->curve(curveId);
}

void plot(const SdrData& data, const QString & title)
{
  SdrThread* pcur = (SdrThread*)QThread::currentThread();
  pcur->plot(data, title);
};
