#include "sdrgui.hh"

#include "../libsig/cpp_monit.hh"

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
  cout<<c_red<<"processus"  <<pcur->GetId()<<"******** "<<time<<c_normal<<endl;
  
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
  cout<<c_red<<"processus"  <<pcur->GetId()<<"******** "<<time<<c_normal<<endl;
  
  sleep(1);
  
  time++;
  
  return -1;
}
int main(int argc, char **argv)
{
  QApplication a(argc, argv);


    SdrGui *  pGui1 = new SdrGui();
    SdrGui *  pGui2 = new SdrGui();
    SdrThread *plot1 = new SdrThread(pGui1,&algo);
    plot1->start();
    SdrThread *plot2 = new SdrThread(pGui2,&algo2);
    plot2->start();


  return a.exec();
}
 
