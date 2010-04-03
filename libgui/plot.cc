#include "plot.hh"


SdrData::SdrData(const SdrData& ref )
{
  if(plot_monit){
    SdrThread* pcur =  (SdrThread*)QThread::currentThread();
    printf("processus");  printf(" %i \n",pcur->GetId());
    
    printf("Appel de constructeur de recopie SdrData \n");
  }
  
  pData = ref.copy();
};



SdrPlot::SdrPlot()
{
    setTitle("no title");
    insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    legend()->setItemMode(QwtLegend::CheckableItem);
    setCanvasBackground(QColor(Qt::white)); 
        
    connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
 	    this, SLOT  (slo_legendChecked(QwtPlotItem *, bool)) );

    // Set axis titles
    setAxisTitle(xBottom, "no title");
    setAxisTitle(yLeft, "no title");

    /***********GRID***********************************************/
    grid = new QwtPlotGrid;
    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->attach(this);

    /***********ZOOMER PROPERTY************************************/
    pZoomer = new SdrZoomer(canvas());
    pZoomer->setEnabled(false);

    /************PANNER PROPERTY***********************************/
    pPanner = new QwtPlotPanner(canvas());
    pPanner->setCursor(Qt::SizeAllCursor/*OpenHandCursor*/);
    pPanner->setEnabled(false);
   
    /************MARKER PROPERTY***********************************/
    pMarker = new SdrMarker(canvas());
    pMarker->setEnabled(false);
    connect(pMarker,SIGNAL(selected (const QwtDoublePoint &)),
	    this   ,SLOT  (addMarker(const QwtDoublePoint &)) );

    /**************************************************************/
    pPicker = new SdrPicker(canvas());
    pPicker->setEnabled(true);

    /**************************************************************/    

    isRunning = true;
}

SdrMainWin::SdrMainWin(int vId, int vGuiId)
  {
    Id = vId; GuiId = vGuiId;
    setWindowTitle("SDRGUI"+QString::number(vGuiId)+"-"+QString::number(vId));
    //     Thread("+QString::number(vGuiId)+")"+"   Figure("+QString::number(vId)+")");

    pPlot = new SdrPlot();
    setCentralWidget(pPlot);

    /***********CONFIG DOCK****************************************/
    pConfig = new PlotConfig("Setup Bar",this);
    addDockWidget(Qt::BottomDockWidgetArea, pConfig);
    pConfig->hide();

    connect(pConfig, SIGNAL(setup()), 
        this, SLOT(setupClicked()) );

    /***********TOOL BAR*******************************************/
    toolBar = new QToolBar("Tool Bar",this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
 
    /*------start button----------------------------*/

    d_startAction = new QAction("STOP", toolBar);
    /*QIcon("smile.png" or start_xpm),*/
    d_startAction->setFont(QFont("Comic Sans MS", 12));
    d_startAction->setCheckable(true);
    d_startAction->setChecked(false);
    d_startAction->setShortcut(QKeySequence("Ctrl+S"));
    slo_showRunning(true);
    toolBar->addAction(d_startAction);

    connect(d_startAction, SIGNAL(toggled(bool)), 
	    pPlot        , SLOT(setPlotState(bool)));
    connect(pPlot        , SIGNAL(sig_showRunning(bool)), 
	    this         , SLOT(slo_showRunning(bool)) );
    
    /*------setup button----------------------------*/
    btnSetup= new QToolButton(toolBar);
  
    btnSetup->setFont(QFont("Comic Sans MS", 12));
    btnSetup->setText("Config");
    btnSetup->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnSetup->setCursor(Qt::PointingHandCursor);
    btnSetup->setToolTip("Show Setup Bar");
    btnSetup->setShortcut(QKeySequence("Ctrl+C"));

    toolBar->addWidget(btnSetup);
    connect(btnSetup, SIGNAL(clicked()), 
	    pConfig, SLOT(show()) );

    /*------Zoomer button------------------*/
    btnZoomer= new QToolButton(toolBar);
  
    btnZoomer->setFont(QFont("Comic Sans MS", 12));
    btnZoomer->setText("Zoomer");
    btnZoomer->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnZoomer->setCursor(Qt::PointingHandCursor);
    btnZoomer->setToolTip("Enable Zoomer");
    btnZoomer->setCheckable(true);
    btnZoomer->setShortcut(QKeySequence("Ctrl+Z"));
    toolBar->addWidget(btnZoomer);
    connect(btnZoomer, SIGNAL(clicked()), 
	    this, SLOT(zoomerClicked()) );
    /*------panner button------------------*/
    btnPanner= new QToolButton(toolBar);
  
    btnPanner->setFont(QFont("Comic Sans MS", 12));
    btnPanner->setText("Panner");
    btnPanner->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnPanner->setCursor(Qt::PointingHandCursor);
    btnPanner->setToolTip("Enable Panner");
    btnPanner->setCheckable(true);
    btnPanner->setShortcut(QKeySequence("Ctrl+P"));
    toolBar->addWidget(btnPanner);
    connect(btnPanner, SIGNAL(clicked()), 
	    this, SLOT(pannerClicked()) );
    /*------marker button------------------*/
    btnMarker= new QToolButton(toolBar);
  
    btnMarker->setFont(QFont("Comic Sans MS", 12));
    btnMarker->setText("Marker");
    btnMarker->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnMarker->setCursor(Qt::PointingHandCursor);
    btnMarker->setToolTip("Enable Marker");
    btnMarker->setCheckable(true);
    btnMarker->setShortcut(QKeySequence("Ctrl+M"));
    
    toolBar->addWidget(btnMarker);
    connect(btnMarker, SIGNAL(clicked()), 
	    this, SLOT(markerClicked()) );
    /*------printer---------------------------------*/
    btnPrint = new QToolButton(toolBar);
  
    btnPrint->setFont(QFont("Comic Sans MS", 12));
    btnPrint->setText("Print");
    btnPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    btnPrint->setCursor(Qt::PointingHandCursor);
    btnPrint->setToolTip("Connect to Printer");

    toolBar->addWidget(btnPrint);
    connect(btnPrint, SIGNAL(clicked()), 
        this, SLOT(printPlot()) );
        
    /*------help------------------------------------*/
    whatsThisAction = QWhatsThis::createAction(toolBar);
    whatsThisAction->setText("Help");
    toolBar->addAction(whatsThisAction);
    initWhatsThis();
 
    /*----------------------------------------------*/
    addToolBar(toolBar);
 
   /**************************************************************/
    resize(600,400);
    show();

  }
void SdrMainWin::printPlot()
{
  QPrinter printer;
  printer.setOrientation(QPrinter::Landscape);
  printer.setOutputFileName("/tmp/spectrogram.pdf");
  QPrintDialog dialog(&printer);
  if ( dialog.exec() )
      {
	pPlot->print(printer);
      }
}
void SdrMainWin::slo_showRunning(bool running)
{
  //d_startAction->setText(running ? "STOP" : "RUN");
  d_startAction->setToolTip(running ? 
			    "Click here to stop plotting"   :
			    "Click here to continue plotting"
			    );
}

void SdrMainWin::setupClicked()
{
  bool isAutoScale = pConfig->isAutoScale();

  if(isAutoScale)
    {
      pPlot->setAxisAutoScale(pPlot->xBottom);
      pPlot->setAxisAutoScale(pPlot->yLeft);;
    }
  else
    {
      double x_min,x_max,y_min,y_max;
      pConfig->getAxisScale(x_min,x_max,y_min,y_max);

      pPlot->setAxisScale(pPlot->xBottom, x_min, x_max);
      pPlot->setAxisScale(pPlot->yLeft, y_min, y_max);
    }

  /*set background color*/
  pPlot->setCanvasBackground(pConfig->getBgColor());
  pPlot->replot();
}
void SdrMainWin::initWhatsThis()
{
    const char *text1 =
        "Zooming is enabled until the selected area gets "
        "too small for the significance on the axes.\n\n"
        "You can zoom in using the left mouse button.\n"
        "The middle mouse button is used to go back to the "
        "previous zoomed area.\n"
        "The right mouse button is used to unzoom completely.";

    const char *text2 =
      "Click on button Stop will stop plotting in realtime, "
      "that gives us a snapshot of the main window.\n"
      "Click on button Run to continue plotting in realtime.";
    
    pPlot->setWhatsThis(text1);
    d_startAction->setWhatsThis(text2);
}

void  SdrThread::run(){
  //int time = 0;
  do{
    worker(argc,argv);
    
    return;
    //msleep(10); 
    
    //time++;
  }while(1);
}



long long  SdrGuiReadTSC() {
   int res[2];                              // store 64 bit result here
   
   #if defined(__GNUC__) && !defined(__INTEL_COMPILER)
   // Inline assembly in AT&T syntax

   #if defined (_LP64)                      // 64 bit mode
      __asm__ __volatile__  (               // serialize (save rbx)
      "xorl %%eax,%%eax \n push %%rbx \n cpuid \n"
       ::: "%rax", "%rcx", "%rdx");
      __asm__ __volatile__  (               // read TSC, store edx:eax in res
      "rdtsc\n"
       : "=a" (res[0]), "=d" (res[1]) );
      __asm__ __volatile__  (               // serialize again
      "xorl %%eax,%%eax \n cpuid \n pop %%rbx \n"
       ::: "%rax", "%rcx", "%rdx");
   #else                                    // 32 bit mode
      __asm__ __volatile__  (               // serialize (save ebx)
      "xorl %%eax,%%eax \n pushl %%ebx \n cpuid \n"
      ::: "%eax", "%ecx", "%edx");
      __asm__ __volatile__  (               // read TSC, store edx:eax in res
      "rdtsc\n"
       : "=a" (res[0]), "=d" (res[1]) );
      __asm__ __volatile__  (               // serialize again
      "xorl %%eax,%%eax \n cpuid \n popl %%ebx \n"
       ::: "%eax", "%ecx", "%edx");
   #endif
   #else
   // Inline assembly in MASM syntax
      __asm {
         xor eax, eax
         cpuid                              // serialize
         rdtsc                              // read TSC
         mov dword ptr res, eax             // store low dword in res[0]
         mov dword ptr res+4, edx           // store high dword in res[1]
         xor eax, eax
         cpuid                              // serialize again
      };
   #endif   // __GNUC__
   
      return *((long long *)res);                   // return result
}
