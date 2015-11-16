#include "sdrgui.hh"
#include <iostream>
#include <unistd.h>

int  algo(int argc, char **argv){
  const int nPoints = 100;
  SdrDataExample* x;
  static int time = 0;
  QString title;
  if(time%2==0)
    {
      x = new  SdrDataExample(::sin, nPoints);
      title = "thread1: sin function";
    }
  else
    {
	x = new  SdrDataExample(::cos, nPoints);
	title = "thread1: cos function";
    }
  
  figure(20);
  plot(*x, title);

  figure(3);
  plot( SdrDataExample(::sqrt, nPoints));

  SdrThread* pcur =  (SdrThread*)QThread::currentThread();
  cout<<"processus"  <<pcur->GetId()<<"******** "<<time<<endl;
  
  sleep(1);
 
  time++;

  return -1;
}

int  algo2(int argc, char **argv){
  const int nPoints = 100;
  SdrDataExample* x;
  static int time = 0;
  if(time%2==0)
    {
      x = new  SdrDataExample(::sin, nPoints);
    }
  else
    {
	x = new  SdrDataExample(::cos, nPoints);
    }
  
  figure(1);
  plot(*x,"plot in thread 2");
  
  SdrThread* pcur =  (SdrThread*)QThread::currentThread();
  cout<<"processus"  <<pcur->GetId()<<"******** "<<time<<endl;
  
  sleep(1);
  
  time++;
  
  return -1;
}
int main(int argc, char **argv)
{
    QApplication a(argc, argv);


    SdrGui   pGui1;
    SdrGui   pGui2;
    SdrThread plot1(&pGui1,&algo);
    plot1.start();
    SdrThread plot2(&pGui2,&algo2);
    plot2.start();

    return a.exec();

}
