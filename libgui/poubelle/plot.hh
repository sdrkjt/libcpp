#ifndef plot_hh
#define plot_hh

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_data.h>
#include <qwt_text.h>
#include <math.h>


#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_layout.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>

#include <stdio.h>
#define plot_monit 0

class SdrData: public QwtData
{

public:

  SdrData(){pData = this;};
  ~SdrData(){
    if(plot_monit)printf("...Delete SdrData\n");
  };
 
  SdrData(const SdrData& ref );
  
  void operator = (const SdrData& origin){
    printf("ERROR: assignment operator undefined,see Class SdrData for more detail \n");
    abort();
  };

  virtual QwtData *copy() const
  {
    if(pData==NULL){ 
      printf("ERROR: SdrData undefined, see Class SdrData for more detail\n");abort();}
    else 
     pData->copy();
  }

  virtual size_t size() const
  {
    if(pData==NULL){ 
      printf("ERROR: SdrData undefined, see Class SdrData for more detail\n");abort();}
    else 
    pData->size();
  }

  virtual double x(size_t i) const
  {
    if(pData==NULL){ 
      printf("ERROR: SdrData undefined, see Class SdrData for more detail\n");abort();}
    else
      return pData->x(i);
  }

  virtual double y(size_t i) const
  {
    if(pData==NULL){
      printf("ERROR: SdrData undefined, see Class SdrData for more detail\n");abort();}
    else    
      return pData->y(i);
  }

private:
  QwtData* pData;

 
};
class SdrDataExample: public SdrData
{

public:
  SdrDataExample(double(*y)(double), size_t size): SdrData(),
    d_size(size),
    d_y(y)
  {
    //pData = this;
  }
  SdrDataExample() : SdrData(){
    d_y = ::sin;
    d_size = 100;
    //pData = this;
  };

  virtual SdrDataExample *copy() const
  {
    if(plot_monit)printf("copy QwtData\n");
    return new SdrDataExample(d_y, d_size);
  }

  size_t size() const
  {
    return d_size;
  }

  double x(size_t i) const
  {
    return 0.1 * i;
  }

  double y(size_t i) const
  {
    return i*d_y(x(i));
  }

private:
  size_t d_size;
  double(*d_y)(double);
};


#include <vector>
#include <QMutex>
#include <QtGui>
#include <qwt_plot_grid.h>

using namespace std;

class SdrPlot : public QwtPlot
{

 Q_OBJECT

public:
  
  vector<QwtPlotCurve *>lCurve;
  vector<QwtPlotMarker*>lMark ;

  SdrPlot(); 
  ~SdrPlot(){
    if(plot_monit)printf("...Delete SdrPlot\n");
    delete grid   ;
    delete pZoomer;
    delete pPanner;
    delete pPicker;
    delete pMarker;
    for(int ii=0;ii<(int)lCurve.size();ii++)
      delete lCurve[ii];
    for(int jj=0;jj<(int)lMark.size();jj++)
      delete lMark[jj];
    lCurve.clear();
    lMark.clear();
  };
  /*interface functions*/  
  int insertCurve(const QString &title){
    QwtPlotCurve* newcurve =  new QwtPlotCurve(title);
    newcurve->attach(this);
    lCurve.push_back(newcurve);
    return lCurve.size();
  }

  void setData(int idcurve,const double *xData, const double *yData, int size){
    lCurve[idcurve-1]->setData(xData,yData,size);
  }

  void setData(int idcurve,const QwtData& data){
    lCurve[idcurve-1]->setData(data);
  }

  void setCurvePen(int idcurve, const QPen &qpen){
    lCurve[idcurve-1]->setPen(qpen);
  }

  /*functions for plot configuration*/

  void setPickerEnabled(bool en){
    pPicker->setEnabled(en);
  };
  void setPannerEnabled(bool en){
    pPanner->setEnabled(en);
  };
  void setZoomerEnabled(bool en){
    pZoomer->setEnabled(en);
  };
  void setMarkerEnabled(bool en){
    pMarker->setEnabled(en);
  };
  void pl_setData(const SdrData& data){
    if(isRunning){
      int curveId = 1;
      setCurvePen(curveId,QPen(Qt::blue));
      setData(curveId,data);
      replot();
    }
  }
  void pl_setTitle(const QString& title){
    if(isRunning)setTitle(title);
  };
public slots:
  void setPlotState(bool ok){
    isRunning = !ok;
    emit sig_showRunning(!ok);
  };

  void addMarker(const QwtDoublePoint &pos){
    //  ...a vertical line 
    QwtPlotMarker *mX = new QwtPlotMarker();
    mX->setLabel(QString::fromLatin1("x = ")+QString::number(pos.x()));
    mX->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    mX->setLineStyle(QwtPlotMarker::VLine);
    mX->setLinePen( QPen( Qt::green, 1, Qt::SolidLine ) );
    mX->setXValue(pos.x());
    mX->attach(this);
    mX->show();
    lMark.push_back(mX);
    replot();
  };
  void slo_legendChecked(QwtPlotItem *p, bool v){
    if(v)p->hide();
    else p->show();
    replot();
  };
signals:
  void sig_showRunning(bool);

private:
  bool          isRunning;
  QwtPlotGrid    *grid   ;
  QwtPlotZoomer  *pZoomer;
  QwtPlotPanner  *pPanner;
  QwtPlotPicker  *pPicker;
  QwtPlotPicker  *pMarker;
};
class PlotConfig: public QDockWidget{
Q_OBJECT

public: 
  PlotConfig(const QString& title,QWidget*parent): QDockWidget(title,parent)
  {
    bgColor = QColor(Qt::white);

    QWidget *contenuDock = new QWidget;
    
    setupButton = new QPushButton("Update");
    //setupButton->setShortcut(QKeySequence());
    QPushButton *bgColorButton = new QPushButton("BG Color");
    QPushButton *hideButton  = new QPushButton("Close");
    hideButton->setShortcut(QKeySequence("Ctrl+C+H"));
    QLabel *labelXAxis = new QLabel("xAxis");
    QLabel *labelYAxis = new QLabel("yAxis");
   
    xAxis_min = new QDoubleSpinBox;
    xAxis_min->setRange(-10000.0,10000.0);
    xAxis_max = new QDoubleSpinBox;
    xAxis_max->setRange(-10000.0,10000.0);
    yAxis_min = new QDoubleSpinBox;
    yAxis_min->setRange(-10000.0,10000.0);
    yAxis_max = new QDoubleSpinBox;
    yAxis_max->setRange(-10000.0,10000.0);

    QHBoxLayout *dockLayout = new QHBoxLayout;
    dockLayout->addWidget(setupButton);
    dockLayout->addWidget(bgColorButton);
    QHBoxLayout *configAxisLayout = new QHBoxLayout;
    configAxisLayout->addWidget(labelXAxis);
    configAxisLayout->addWidget(xAxis_min);configAxisLayout->addWidget(xAxis_max);
    configAxisLayout->addWidget(labelYAxis);
    configAxisLayout->addWidget(yAxis_min);configAxisLayout->addWidget(yAxis_max);
    
    configAxis = new QGroupBox("Axis Scale");
    configAxis->setCheckable(true);
    configAxis->setChecked(false);
    configAxis->setLayout(configAxisLayout);
    dockLayout->addWidget(configAxis);

    dockLayout->addWidget(hideButton);

    contenuDock->setLayout(dockLayout);
    setWidget(contenuDock);

    connect(setupButton, SIGNAL(clicked()), 
	    this, SLOT(setupClicked()) );
    connect(bgColorButton, SIGNAL(clicked()), 
	    this, SLOT(colorClicked()) );
    connect(hideButton, SIGNAL(clicked()), 
    	    this, SLOT(hide()) );
  }

  ~PlotConfig(){
    if(plot_monit)printf("...Delete PlotConfig\n");
    delete setupButton;
    delete xAxis_min ;
    delete xAxis_max ;
    delete yAxis_min ;
    delete yAxis_max ;
    delete configAxis;

  }
  void getAxisScale(double &x_min,double &x_max,double &y_min, double &y_max){
    x_min = xAxis_min->value();
    x_max = xAxis_max->value();
    y_min = yAxis_min->value();
    y_max = yAxis_max->value();
  }
  bool isAutoScale(){
    return !configAxis->isChecked();
  }

  QColor getBgColor(){
    return bgColor;
  }
signals:
  void setup();
 
public slots:
  void setupClicked(){emit setup();};
  void colorClicked(){
    bgColor = QColorDialog::getColor(Qt::white, this);
  };

private:
  QPushButton          *setupButton;

  QGroupBox            *configAxis;
  QDoubleSpinBox       *xAxis_min ;
  QDoubleSpinBox       *xAxis_max ;
  QDoubleSpinBox       *yAxis_min ;
  QDoubleSpinBox       *yAxis_max ;

  QColor                bgColor   ;

};

#include "start.xpm"
class SdrMainWin: public QMainWindow 
{
 Q_OBJECT

public:
  SdrMainWin(int vId,int GuiId);
  ~SdrMainWin(){
    if(plot_monit)printf("...Delete SdrMainWin\n");
    delete pPlot;
    delete pConfig;
    delete d_startAction;
    delete btnSetup ;
    delete btnPanner;
    delete btnMarker;
    delete btnZoomer;
    delete btnPrint ;
    delete whatsThisAction;
    delete toolBar;
  }
  int GetId(){return Id;}; 

  /********PLOT************/
  int insertCurve(const QString &title){
   return pPlot->insertCurve(title);
  }
  void pl_setData(const SdrData& data){
    pPlot->pl_setData(data);
  };
  void pl_setTitle(const QString& title){
    pPlot->pl_setTitle(title);
  };
  /************************/

public slots:
  void printPlot();
  void slo_showRunning(bool running);
  void setupClicked();
  void zoomerClicked(){
    bool checked = btnZoomer->isChecked();
    pPlot->setZoomerEnabled(checked);
    if(checked)
      {
	pPlot->setPickerEnabled(false);
	pPlot->setPannerEnabled(false);
	pPlot->setMarkerEnabled(false);
	
	btnMarker->setChecked(false);
	btnPanner->setChecked(false);
      }
    else
      {
	pPlot->setPickerEnabled(true);
      }
  };
  void pannerClicked(){
    bool checked = btnPanner->isChecked();
    pPlot->setPannerEnabled(checked);
    if(checked)
      {
	pPlot->setPickerEnabled(false);
	pPlot->setZoomerEnabled(false);
	pPlot->setMarkerEnabled(false);

	btnZoomer->setChecked(false);
	btnMarker->setChecked(false);
      }
    else
      {
	pPlot->setPickerEnabled(true);
      }
  };
  void markerClicked(){
    bool checked = btnMarker->isChecked();
    pPlot->setMarkerEnabled(checked);
    if(checked)
      {
	pPlot->setPickerEnabled(false);
	pPlot->setZoomerEnabled(false);
	pPlot->setPannerEnabled(false);
	
	btnPanner->setChecked(false);
	btnZoomer->setChecked(false);
      }
    else
      {
	pPlot->setPickerEnabled(true);
      }
  };
private:
  int                 Id;
  int              GuiId;//=ThreadId
  SdrPlot         *pPlot;
  
  QAction *d_startAction;

  PlotConfig    *pConfig;
  
  QToolBar    *toolBar  ;
  QToolButton *btnSetup ;
  QToolButton *btnPanner;
  QToolButton *btnMarker;
  QToolButton *btnZoomer;
  QToolButton *btnPrint ;
  QAction     *whatsThisAction;

  void initWhatsThis();
};


class SdrGui : public QObject {
  Q_OBJECT

public:
  SdrGui(){
    CurFigId = 1; ThreadId = -1;
  }
  ~SdrGui(){
    if(plot_monit)printf("...Delete SdrGui\n");
    for(int ii=0;ii<(int)lFigure.size();ii++)
      delete lFigure[ii];
    lFigure.clear();
  }
  void setThreadId(int id){
    ThreadId = id;
  }
  SdrMainWin* GetFigure(int Id){
    for(int ii=0;ii<(int)lFigure.size();ii++)
      if(lFigure[ii]->GetId()==Id)return lFigure[ii];
    return NULL;
  };

public slots:
  
  void slo_figure(int Id)
  { CurFigId = Id;};

  void slo_plot(const SdrData& data, const QString& title)
  {
    if(plot_monit)printf("Process slots\n");
    SdrMainWin* pfig = GetFigure(CurFigId);
    if(pfig==NULL)  {
      pfig = new SdrMainWin(CurFigId,ThreadId) ;
      pfig->insertCurve("function");
      lFigure.push_back(pfig);
    }
    pfig->pl_setTitle(title);
    if(plot_monit)printf("Set Data\n");
    pfig->pl_setData(data);
  };

private:
  vector<SdrMainWin*> lFigure;
  int CurFigId;
  int ThreadId;
};



#include <QThread>
long long SdrGuiReadTSC();//for time measurement

class SdrThread  : public QThread
{
  Q_OBJECT

public:

  int (*worker) (int, char **);

  SdrThread(SdrGui* pGui_v,
	    int (*function) (int, char **), 
	    int vargc=-1, char **vargv=NULL ){

    pGui = pGui_v;

    worker = function;
    argc = vargc;
    argv = vargv;

    long long init_time = SdrGuiReadTSC();
    for(int ii=0;ii<100;ii++) timer[ii] = init_time;

    static int count = 1;

    //qRegisterMetaType<SdrData>("SdrData");
    qRegisterMetaType<SdrData>("SdrData");
    QObject::connect(this, SIGNAL(sig_figure(int)),
		     pGui, SLOT  (slo_figure(int)));
    QObject::connect(this, SIGNAL(sig_plot(const SdrData& , const QString&)),
		     pGui, SLOT  (slo_plot(const SdrData& , const QString&)));

    id = count;
    /*!one SdrGui per thread
      =>SdrGui's id is thread's id 
     */
    pGui->setThreadId(id);
    count++;

  }

  int GetId(){ return id; }

  void plot(const SdrData& data, const QString& title="notitle"){
    /*!22-12-09
      -use timer for slow down the signal emission
      -recommendation :
       use getchar() before and after plot() if we want to be sure that 
       the plot command is taken in account
      -by default: the service is not guaranteed
     */
    long long  curTime = SdrGuiReadTSC();
    if((curTime-timer[curFig])>(1<<29)){
      if(plot_monit)printf("Emit signal plot\n");
      emit sig_plot(data,title);
      timer[curFig] = curTime;
    }
  }
  void figure(int Id){
    if(Id>=100){printf("figure Id should be smaller than 100: %i\n",Id);exit(1);}
    curFig = Id;
    emit sig_figure(Id);
  }

signals:
  void sig_plot(const SdrData& data, const QString& title);
  void sig_figure(int Id);

private:
  int                      id;
  int                    argc;
  char                 **argv;
  int                  curFig;
  long long         timer[99]; 
  SdrGui                *pGui;

  virtual void run();

};

class SdrZoomer: public QwtPlotZoomer
{
public:
    SdrZoomer(QwtPlotCanvas *canvas):
        QwtPlotZoomer(canvas)
    {
      /*use RightButton to unzoom step by step*/
      //setMousePattern(QwtEventPattern::MouseSelect2,
      //		      Qt::RightButton, Qt::ControlModifier);
      //setMousePattern(QwtEventPattern::MouseSelect3,
      //		      Qt::RightButton);
      
      const QColor c( Qt::blue);
      setRubberBandPen(QPen(c, 2, Qt::DotLine));
      setTrackerPen(c);
      
      setTrackerMode(AlwaysOn);
    }

    virtual QwtText trackerText(const QwtDoublePoint &pos) const
    {
        QColor bg(Qt::white);
        bg.setAlpha(200);
        QwtText text = QwtPlotZoomer::trackerText(pos);
        text.setBackgroundBrush( QBrush( bg ));
        return text;
    }
};
#include <qwt_picker.h>
class SdrPicker: public QwtPlotPicker
{
public:
    SdrPicker(QwtPlotCanvas *canvas):
        QwtPlotPicker(canvas)
    {
      setRubberBandPen(QPen(Qt::red, 1, Qt::SolidLine));
      setTrackerPen(QColor(Qt::red));
    
      setRubberBand(QwtPicker::CrossRubberBand);

      setSelectionFlags(QwtPicker::PointSelection|QwtPicker::DragSelection);
   
      setTrackerMode(AlwaysOn);
    }

    virtual QwtText trackerText(const QwtDoublePoint &pos) const
    {
        QColor bg(Qt::white);
        bg.setAlpha(200);
        QwtText text = QwtPlotPicker::trackerText(pos);
        text.setBackgroundBrush( QBrush( bg ));
        return text;
    }

};

#include <qwt_plot_marker.h>
class SdrMarker: public QwtPlotPicker
{
Q_OBJECT
public:
    SdrMarker(QwtPlotCanvas *canvas):
        QwtPlotPicker(canvas)
    {
      setRubberBandPen(QPen(Qt::green, 1, Qt::SolidLine));
      setTrackerPen(QColor(Qt::blue));
    
      setRubberBand(QwtPicker::VLineRubberBand);

      setSelectionFlags(QwtPicker::PointSelection|QwtPicker::DragSelection);
   
      setTrackerMode(AlwaysOn);
    }

    virtual QwtText trackerText(const QwtDoublePoint &pos) const
    {
        QColor bg(Qt::white);
        bg.setAlpha(200);
        QwtText text = QwtPlotPicker::trackerText(pos);
        text.setBackgroundBrush( QBrush( bg ));
        return text;
    }

};

#endif
