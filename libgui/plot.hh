#ifndef plot_hh
#define plot_hh

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
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
     return pData->copy();
  }

  virtual size_t size() const
  {
    if(pData==NULL){ 
      printf("ERROR: SdrData undefined, see Class SdrData for more detail\n");abort();}
    else 
    return pData->size();
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
struct CurveStruct{
  int                Id;
  QwtPlotCurve *   pObj;
};
class SdrPlot : public QwtPlot
{

 Q_OBJECT

public:
  
  vector<CurveStruct   >lCurve;
  vector<QwtPlotMarker*>lMark ;

  SdrPlot(); 
  ~SdrPlot(){
    if(plot_monit)printf("...Delete SdrPlot\n");
    delete grid   ;
    delete pZoomer;
    delete pPanner;
    delete pPicker;
    delete pMarker;
    for(int ii=0;ii<(int)lCurve.size();ii++){
      delete lCurve[ii].pObj;
    }
    for(int jj=0;jj<(int)lMark.size();jj++)
      delete lMark[jj];
    lCurve.clear();
    lMark.clear();
  };
  /*interface functions*/  
  void createCurve(int curveId,const QString &title){
    int pos = getPosInList(curveId);
    /*current version supports multiple curves per figure*/
    if(pos==-1){ //if curve is not exist, create it
      QwtPlotCurve* newcurve =  new QwtPlotCurve(title);
      /*set default parameters*/
      //newcurve->setSymbol(QwtSymbol());
      newcurve->setStyle(QwtPlotCurve::Lines);
      newcurve->setPen(QPen(Qt::blue,1));
      newcurve->attach(this);
      CurveStruct newCurveStruct;
      newCurveStruct.Id   = curveId;
      newCurveStruct.pObj = newcurve;
      lCurve.push_back(newCurveStruct);
    }
    else{
      lCurve[pos].pObj->setTitle(title);
    }
  }

  void setData(int idcurve,const double *xData, const double *yData, int size){
    lCurve[getPosInList(idcurve)].pObj->setData(xData,yData,size);
  }

  void setData(int idcurve,const QwtData& data){
    lCurve[getPosInList(idcurve)].pObj->setData(data);
  }

  void setCurvePen(int idcurve, const QString &colStr,const QString &symStr,const QString &styStr){
    QwtSymbol sym;
    QwtSymbol::Style symId;
    if(symStr==QString("+")) 
      symId = QwtSymbol::Cross;
    else if(symStr==QString("x"))
      symId = QwtSymbol::XCross;
    else if(symStr==QString("*"))
      symId = QwtSymbol::Star1;
    else if(symStr==QString("o"))
      symId = QwtSymbol::Ellipse;
    else { 
      symId = QwtSymbol::NoSymbol;
    }
    sym.setStyle(symId);
    sym.setPen(QPen(Qt::blue,1));
    sym.setSize(7);
    
    QPen qpen;
    if(colStr=="rd")
      qpen = QPen(Qt::red,1,Qt::DotLine);
    else if(colStr=="we")
      qpen = QPen(Qt::white,1);
    else if(colStr=="be")
      qpen = QPen(Qt::blue,1);
    else if(colStr=="bk")
      qpen = QPen(Qt::black,1); 
    else if(colStr=="gn")
      qpen = QPen(Qt::green,1);
    else if(colStr=="gy")
      qpen = QPen(Qt::gray,1);
    else if(colStr=="yw")
      qpen = QPen(Qt::yellow,1);
    else if(colStr=="cn")
      qpen = QPen(Qt::cyan,1);
    else if(colStr=="ma")
      qpen = QPen(Qt::magenta,1);
    else {
      qpen = QPen(Qt::blue,1);
    }

    QwtPlotCurve::CurveStyle styId;
    if(styStr=="Lines")
      styId = QwtPlotCurve::Lines;
    else if(styStr=="Sticks")
      styId = QwtPlotCurve::Sticks;
    else if(styStr=="Steps")
      styId = QwtPlotCurve::Steps;
    else if(styStr=="Dots")
      styId = QwtPlotCurve::Dots;
    else{
      styId = QwtPlotCurve::NoCurve;
    }

    int pos = getPosInList(idcurve);

    lCurve[pos].pObj->setSymbol(sym);

    lCurve[pos].pObj->setStyle(styId);
	   
    lCurve[pos].pObj->setPen(qpen);
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
  void pl_setPen(int curveId,const QString &colStr,const QString &symStr,const QString &styStr){
    if(isRunning)setCurvePen(curveId,colStr,symStr,styStr);
  }
  void pl_setData(const SdrData& data, int curveId){
    if(isRunning){
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

  int getPosInList(int curveId){
    int pos = -1;
    for(int ii=0;ii<(int)lCurve.size();ii++){
      if(lCurve[ii].Id==curveId){ pos = ii; break;}
    }
    return pos;
  };
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
  void createCurve(int curveId,const QString &title){
    pPlot->createCurve(curveId,title);
  }
  void pl_setPen(int curveId,const QString &colStr,const QString &symStr,const QString &styStr){
    pPlot->pl_setPen(curveId,colStr,symStr,styStr);
  }
  void pl_setData(const SdrData& data, int CurveId){
    pPlot->pl_setData(data,CurveId);
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
  
  void slo_figure(int Id, const QString& title)
  {
    if(plot_monit)printf("Process slots figure\n");
    CurFigId = Id;
    CurFig = GetFigure(CurFigId);
    if(CurFig==NULL)  {
      CurFig = new SdrMainWin(CurFigId,ThreadId) ;
      lFigure.push_back(CurFig);
    }
    CurFig->pl_setTitle(title);
  };
  void slo_setpen(int curveId,const QString &colStr,const QString &symStr,const QString &styStr){
    if(plot_monit)printf("Process slots setpen\n");
    CurFig->createCurve(curveId,"notitle");//create curve if not exist
    CurFig->pl_setPen(curveId,colStr,symStr,styStr);
  };
  void slo_plot(int curveId,const SdrData& data, const QString& title)
  {
    if(plot_monit)printf("Process slots plot\n");
    CurFig->createCurve(curveId,title);//create curve if not exist

    if(plot_monit)printf("Set Data\n");
    CurFig->pl_setData(data,curveId);
  };

private:
  vector<SdrMainWin*> lFigure;
  int                CurFigId;
  SdrMainWin*         CurFig ;
  int                ThreadId;
};



#include <QThread>
long long SdrGuiReadTSC();//for time measurement
struct timer{
  int     figid;
  int     curid;
  long long val;
};

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

    realtime = false;

    static int count = 1;

    //qRegisterMetaType<SdrData>("SdrData");
    qRegisterMetaType<SdrData>("SdrData");
    QObject::connect(this, SIGNAL(sig_figure(int,const QString&)),
		     pGui, SLOT  (slo_figure(int,const QString&)));
    QObject::connect(this, SIGNAL(sig_setpen(int,const QString& , const QString&, const QString&)),
		     pGui, SLOT  (slo_setpen(int,const QString& , const QString&, const QString&)));
    QObject::connect(this, SIGNAL(sig_plot(int,const SdrData& , const QString&)),
		     pGui, SLOT  (slo_plot(int,const SdrData& , const QString&)));

    id     = count;
    curFig = 0;curCurve = 0;
    /*!one SdrGui per thread
      =>SdrGui's id is thread's id 
     */
    pGui->setThreadId(id);
    count++;

  }

  int GetId(){ return id; }

  void plot(const SdrData& data, const QString& title="notitle"){
    /*!22-12-09
      -use timer in realtime mode to slow down the signal emission
      -recommendation :
       use getchar() before and after plot() if we want to be sure that 
       the plot command is taken in account
      -by default: the service is not guaranteed
     */
    if(realtime){
      long long  curTime = SdrGuiReadTSC();
      int pos = getTimerPos(curFig,curCurve);
      if(pos==-1){//create new timer
	timer newtimer; newtimer.figid = curFig; newtimer.curid = curCurve;
	newtimer.val = curTime; //init timing
	lTimer.push_back(newtimer);
	if(plot_monit)printf("Emit signal plot\n");
	emit sig_plot(curCurve,data,title);
      }
      else if((curTime-lTimer[pos].val)>(1<<29)){
	lTimer[pos].val = curTime; //update timing
	if(plot_monit)printf("Emit signal plot\n");
	emit sig_plot(curCurve,data,title);
      }
    }
    else{
      if(plot_monit)printf("Emit signal plot\n");
      emit sig_plot(curCurve,data,title);
    }
  }
  void figure(int Id,const QString& title="notitle"){
    curFig   = Id;
    curCurve = 0; //0 is the default curve
    if(plot_monit)printf("Emit signal figure\n");
    emit sig_figure(curFig,title);
  }
  void curve(int curveId){
    curCurve = curveId;
  };
  void setpen(const QString &colStr,const QString &symStr,const QString &stySt){
    if(plot_monit)printf("Emit signal setpen\n");
    emit sig_setpen(curCurve,colStr,symStr,stySt);
  };
  void SetRealTime(bool val){
    realtime = val;
  };
signals:
  void sig_plot(int curveId,const SdrData& data, const QString& title);
  void sig_figure(int Id, const QString& title);
  void sig_setpen(int curveId,const QString &colStr,const QString &symStr,const QString &stySt);
private:
  int                      id;
  int                    argc;
  char                 **argv;
  int                  curFig;
  int                curCurve;
  bool               realtime;
  vector<timer>        lTimer; 
  SdrGui                *pGui;

  virtual void run();

  int getTimerPos(int figid,int curid){
    int pos = -1;
    for(int ii=0;ii<(int)lTimer.size();ii++){
      if((lTimer[ii].figid==figid) && (lTimer[ii].curid==curid)){
	pos = ii; break;
      }
    }
    return pos;
  };
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
