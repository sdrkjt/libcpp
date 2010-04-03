***************************
Structure de DREAM
****************************
*1 thread pour QT, 1 ou plusieurs thread pour la partie de Calcul
*thread QT est le thread principale (avec main())
*les donnés  de la partie Calcul sont accessible pour QT grace à Mutex: 
 les fonctions d'interface sont définies dans la partie calcul. Ces fonctions doivent commencer par lock() et finir par unlock() 
-les variables partagées entre demod et QT sont associées à 1 Mutex.
-la partie demod update les valeurs de ces variables à chaque symbol.
-la partie QT consulte périodiquement ces variables et update ses fenêtre. 
*il n'y a pas de plot dans la partie calcul.

***autre solution constaté***
-La partie calul emit les signaux (demande:  de créer de novelle fenêtre, de updater les fenêtres existantes, ...).
-La partie QT traite ces signaux par ses slots.
-la synchronisation se fait par 1 flag: à chaque fois la demod emit un signal, elle se met dans un état de wait et checker la flag pour savoir si sa demande a été traité. QT traite la demande, quand elle la finit, elle signale  à la partie calul par la flag. L'acces à flag se protège par 1 Mutex.
****************************

QT Thread:
**Why on earth are you creating and execing a dialog in a thread? You can't do that with Qt, it must be created in the main thread - and calling a QThread 'mainthread' does not make it the main thread. The main thread is where your program starts, any threads that you explicitly create are separate to the main thread. Move the creation of the dialog to the main thread.
**It's about the fact that one thread is not allowed to directly access widgets created by another thread (as far as I understand, see http://doc.trolltech.com/4.4/qthread.html#details
My solution was to use QCoreApplication::postEvent with a subclassed event and handle that event in my widget. 
**Only the main (GUI) thread is allowed to update the GUI. The signal/slot mechanism is synchronous, meaning that all slots that are connected to a signal from a thread will be executed in the context of the thread that emitted the signal.  
The solution is to use QCustomEvent, use a postEvent in your thread, and reimplement ::customEvent() in the recieving class (in the main thread). If you subclass QCustomEvent the event can have data if that is necessary. This way your thread can notify the main thread that new data is available
**Only the main event loop thread (GUI Thread) can interact with widgets, but the second thread can send events to objects which will be executed in the context of the event loop thread See QApplication::sendEvent and QApplication::postEvent and search for this terms here in the forum as well.


** qthread can emit signals to the maingui. These signals are stocked into a queue in the maingui,  all parameters are copied at the moment where qthread emits the signal (see qt4 manual for more detail).  The main event loop will process  this queue in order.

*******libgui**************
I.Architecture and Property
-One SdrGui per Thread:
 Thread emits signals (plot commands), SdrGui receives&process  signals
-All SdrGuis are belong to the maingui
-Each SdrGui manages a list of windows (SdrMainWin)
-Each window manages a list of curves
-A timer is associated to each curve: this is used for realtime mode to slow down the signal emission from threads, the default setting value is (2^29) cpuclocks between 2 updates 

-signals-slots: sig_plot, sig_figure, sig_setpen
 
II.APIs: plot,figure,curve,setpen,SetRealTime
**figure args are:
-figure id
-title
**curve arg is: curve id
**setpen args arg:
-color custom: blue is the default color
 [rd]red [we]white [be]blue [bk]black [gn]green [gy]gray [yw]yellow [cn]cyan [ma]magenta
-symbol custom: + x * o (no symbol in default mode)
-style custom: Lines Sticks Steps Dots, NoCurve is default value
**plot args are:
-data
-title

***************************
