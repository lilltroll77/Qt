#include "mainwindow.h"
#include <QTime>

void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QProgressDialog progress("Waiting for XMOS to reply" , "Cancel" ,0,2 , this);

    msgBoxAbout = new QMessageBox;
    msgBoxAbout->setText(QString("XMOS-GUI ver: %1\nCompiled %2\n\Written in C++\nBy Mikael Bohman 2013-2014").arg(VERSION).arg(COMPILEDATE) );
    msgBoxAbout->setFixedWidth(640);
    msgBoxAbout->setFixedHeight(480);
    msgBoxAbout->setWindowTitle(tr("About"));

    QCheckBox *cb = new QCheckBox("Show phase");
    msgBoxGraph.setCheckBox(cb);
    //msgBoxGraph.set
    msgBoxGraph.setFixedWidth(640);
    msgBoxGraph.setFixedHeight(480);
    msgBoxGraph.setWindowTitle(tr("Graph options"));

/*** MENU BAR ***/
    menubar = new QMenuBar(this);
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->setObjectName("fileMenu");
    networkMenu = new QMenu(tr("&Network") , this);
    networkMenu->setObjectName("networkMenu");
    helpMenu = new QMenu(tr("&Help"), this);
    options = new QMenu(tr("&Options"), this);
    //QIcon(":/images/new.png")

    openAction = fileMenu->addAction(tr("&Open ..."));
    connect(openAction,SIGNAL(triggered()),this, SLOT(slot_open()) );

    saveAsAction = fileMenu->addAction(tr("&Save as..."));
    connect(saveAsAction , SIGNAL(triggered()),this,SLOT(slot_saveas()) );

    exitAction = fileMenu->addAction(tr("E&xit"));
    connect(exitAction , SIGNAL(triggered()),this, SLOT( close()) ) ;

    networkAction = networkMenu->addAction(tr("Network settings"));
    connect(networkAction , SIGNAL(triggered()) , this , SLOT(slot_networkSettings()));

    syncFromHostAction = networkMenu->addAction(tr("Sync Computer->HW"));
    connect(syncFromHostAction , SIGNAL( triggered(bool)) , this , SLOT(slot_syncFromHost(bool)) );

    syncToHostAction = networkMenu->addAction(tr("Sync HW->Computer"));
    //syncToHostAction->setEnabled(false);
    connect(syncToHostAction  , SIGNAL(triggered(bool)) , this , SLOT(slot_syncToHost(bool)) );

    aboutAction= helpMenu->addAction(tr("&About"));
    connect(aboutAction , SIGNAL(triggered()),this, SLOT(slot_about()) );

    graphAction = options->addAction(tr("Graph"));
    connect(graphAction , SIGNAL(triggered()) ,this , SLOT(slot_graph_options()) );

    menubar->addMenu(fileMenu);
    menubar->addMenu(options);
    menubar->addMenu(networkMenu);
    menubar->addMenu(helpMenu);
    setMenuBar(menubar);

    /*** STAUS BAR ***/

    statusbar = new QStatusBar;
    statusbar->setObjectName("statusbar");
    statusbar->showMessage(tr("Not connected"));
    setStatusBar(statusbar);
    setWindowTitle("Alfa release");


    /*** CENTRAL WIDGET ***/

    central_widget=new Widget(this);
    setCentralWidget(central_widget);
    central_widget->setObjectName("central_widget");
   // connect(central_widget-> , SIGNAL() , this ,SLOT() );

/*** TOOLBARS ***/
    QIcon *openIcon = new QIcon(":/icons/images/open.png");
    QIcon *saveIcon = new QIcon(":/icons/images/save.png");
    QIcon *syncIcon = new QIcon(":/icons/images/Sync.png");
    QIcon *mainIcon = new QIcon(":/icons/images/artsfftscope.png");

    setWindowIcon(*mainIcon);

    fileToolBar = addToolBar(tr("File"));
    openActionToolbar=fileToolBar->addAction(*openIcon,tr("Open File"));
    connect(openActionToolbar , SIGNAL(triggered()),this, SLOT(slot_open()) );

    saveAsActionToolbar=fileToolBar->addAction(*saveIcon,tr("Save File"));
    connect(saveAsActionToolbar , SIGNAL(triggered()),this,SLOT(slot_saveas()) );

    fileToolBar->addAction(*syncIcon,tr("Sync with XMOS"));
    //
    currentProgram=0;
    //central_widget->main_tab->radiobuttons[currentProgram]->setChecked(true);

   }

void MainWindow::slot_about(){
   msgBoxAbout->exec();
}

void MainWindow::slot_graph_options(){
    msgBoxGraph.exec();
}


void MainWindow::slot_open(){
  QString fileName;
  QFileDialog *filedialog=new QFileDialog;
  filedialog->setDefaultSuffix("DSPsettings");
  fileName=filedialog->getOpenFileName(this , tr("Open File"),"","DSPsettings");
  QFile file(fileName);
  file.open(QIODevice::ReadOnly);
  QDataStream in(&file);
  quint64 magic;
  qint32 version;
  QString str;
  quint16 channels;
  quint16 sections;
  int k_choice , type , mute;
  bool state;
  int polarity;
  double fc, Q ,gain , delay ,p,k;
  QString displayText;
  bool blockSignals = true;

  in >> magic;
  if(magic != MAGIC_FILECHECKNUMBER){
      msgBoxError.setText(tr("BAD FILE FORMAT"));
      msgBoxError.setWindowTitle(tr("FILE ERROR"));
      msgBoxError.exec();
      return;
}
  in >> version;
  if(version<FILEVERSION){
      msgBoxError.setText(tr("FILE TOO OLD"));
      msgBoxError.setWindowTitle(tr("FILE ERROR"));
      msgBoxError.exec();
      return;
  }
  in >> channels;
  in >> sections;
  in >> p; central_widget->mixer_tab->setP(p , blockSignals);
  in >> k; central_widget->mixer_tab->setK(k , blockSignals);
  in >> k_choice; central_widget->mixer_tab->setK_type((k_choice_t) k_choice , blockSignals);
  in >> fc; central_widget->eq_tab->setLinkedFc(fc , blockSignals);
  in >> gain; central_widget->eq_tab->setPreGain(gain , blockSignals);

  for(int ch=0 ; ch<(int) channels ; ch++){
      in >> gain; central_widget->dac_tab->channel[ch]->setGain(gain , blockSignals);
      in >> polarity; central_widget->dac_tab->channel[ch]->setPolarity( (polarity_t) polarity , blockSignals);
      in >> mute; central_widget->dac_tab->channel[ch]->setMute((mute_t) mute , blockSignals);
      in >> displayText ; central_widget->dac_tab->channel[ch]->channelAlias->setText(displayText);
      in >> delay; central_widget->eq_tab->channel[ch]->setDelay(delay , blockSignals);
   for(int sec=0 ; sec<(int) sections ; sec++){
        in >> state; central_widget->eq_tab->channel[ch]->eqSection[sec]->setFilterActive(state , blockSignals);
        in >> type; central_widget->eq_tab->channel[ch]->eqSection[sec]->setFilterType((filterType_t) type , blockSignals );
        in >> fc; central_widget->eq_tab->channel[ch]->eqSection[sec]->setFc(fc , blockSignals);
        in >> state; central_widget->eq_tab->channel[ch]->eqSection[sec]->setLinked(state , blockSignals);
        in >> Q; central_widget->eq_tab->channel[ch]->eqSection[sec]->setQ(Q , blockSignals);
        in >> gain;  central_widget->eq_tab->channel[ch]->eqSection[sec]->setGain(gain , blockSignals);
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->B[0];
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->B[1];
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->B[2];
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->A[0];
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->A[1];

         //update freq response data in struct but do not redraw
        central_widget->eq_tab->channel[ch]->eqSection[sec]->updateSettingsAndPlot(false);
    }
   central_widget->eq_tab->channel[ch]->recalc_graph();
  }
  central_widget->main_tab->setMode(USER);
  syncFromHost();
  file.close();

}


void MainWindow::slot_saveas(){
  int i=0;
  QString fileName;
  QFileDialog *filedialog=new QFileDialog;
  filedialog->setDefaultSuffix("DSPsettings");
  fileName=filedialog->getSaveFileName(this , tr("Save current program to file"),"","DSPsettings");
  QFile file(fileName);
  file.open(QIODevice::WriteOnly);
  QDataStream out(&file);
  out << (quint64) MAGIC_FILECHECKNUMBER;
  out << (quint32) FILEVERSION;
  //out << QString("Cracy Horse File");
  out << (quint16) CHANNELS;
  out << (quint16) SECTIONS;
  //out << central_widget->main_tab->knob_bremen3D->Value();

  out << central_widget->mixer_tab->getP();
  out << central_widget->mixer_tab->getK();
  out << (int) central_widget->mixer_tab->getK_type();
  out << central_widget->eq_tab->getLinkedFc();
  out << central_widget->eq_tab->getPreGain();

  for(int ch=0 ; ch<CHANNELS ; ch++){
      out << central_widget->dac_tab->channel[ch]->getGain();
      out << (int) central_widget->dac_tab->channel[ch]->getPolarity();
      out << (int) central_widget->dac_tab->channel[ch]->getMute();
      out << central_widget->dac_tab->channel[ch]->channelAlias->displayText();
      out << central_widget->eq_tab->channel[ch]->getDelay();
      for(int sec=0 ; sec<SECTIONS ; sec++){
          out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->getFilterActive();
          out << (int) central_widget->eq_tab->channel[ch]->eqSection[sec]->getFilterType();
          out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->getFc();
          out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->getLinked();
          out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->getQ();
          out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->getGain();
          out << central_widget->eq_tab->channel[ch]->eqSection[sec]->B[0];
          out << central_widget->eq_tab->channel[ch]->eqSection[sec]->B[1];
          out << central_widget->eq_tab->channel[ch]->eqSection[sec]->B[2];
          out << central_widget->eq_tab->channel[ch]->eqSection[sec]->A[0];
          out << central_widget->eq_tab->channel[ch]->eqSection[sec]->A[1];
      }
}


  file.close();
}

void MainWindow::waitForPING(){

    progress.setWindowModality(Qt::WindowModal);
    //Wait for XMOS to reply
    progress.setMinimumDuration(0);
    progress.setValue(0);
    progress.repaint();
    progress.show();
    statusbar->showMessage("Waiting for XMOS to reply");
    central_widget->main_tab->slot_pingXMOS();
    QString str;
      while(1){
        str = statusBar()->currentMessage();
        if(str.startsWith("Connected") || progress.wasCanceled())
            break;
        central_widget->main_tab->slot_pingXMOS();
        delay(500);
        //qDebug()<<"PING";
     }
      progress.setValue(1);
}


void MainWindow::syncFromHost(){
    int mode=central_widget->main_tab->getMode();
    if(mode==PRESET){
        central_widget->main_tab->sendProgram();
        central_widget->main_tab->getLock_fs();
    }else{
    bool mute = central_widget->main_tab->getMuteState();
    central_widget->main_tab->slot_sendMute(true);
    delay(250);
    for(int ch=0 ; ch<8 ; ch++){
        central_widget->eq_tab->channel[ch]->slot_sendEQChannelSettings();
       delay(10);
    }
    central_widget->eq_tab->slot_sendPreGain();
    central_widget->dac_tab->slot_sendDACsettings();
    central_widget->mixer_tab->sendMixerSettings();
    central_widget->main_tab->slot_sendMute(mute);
    central_widget->main_tab->sendSettings();
    central_widget->main_tab->getLock_fs();
    }
}

void MainWindow::slot_syncFromHost(bool button){
  //qDebug()<<"Sync from host="<<button;
  QMessageBox* msgBox = new QMessageBox;
  msgBox->setWindowTitle("Sync");
  msgBox->setText("This action will syncronize all settings from this computer to the hardware.\nThe current settings in the hardware will be overwritten.\n");
  msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Discard);
  msgBox->setDefaultButton(QMessageBox::Discard);
  if(msgBox->exec() == QMessageBox::Ok){
    waitForPING();
    syncFromHost();
  }
  delete msgBox;
}

void MainWindow::slot_syncToHost(bool button){
    qDebug()<<"Sync to host="<<button;
    QMessageBox* msgBox = new QMessageBox;
    msgBox->setWindowTitle("Sync");
    msgBox->setText("This action will syncronize all settings from the hardware to this computer.\nThe current settings on this computer will be overwritten.\n");
    msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Discard);
    msgBox->setDefaultButton(QMessageBox::Discard);
    if(msgBox->exec() == QMessageBox::Ok)
    {
        waitForPING();
        central_widget->main_tab->slot_syncFromXMOS();
    }

    delete msgBox;
  }

void MainWindow::slot_networkSettings(){
    QMessageBox* msgBox = new QMessageBox;
    msgBox->setFixedWidth(480);
    msgBox->setFixedWidth(320);
    msgBox->setWindowTitle("Network settings");
    msgBox->setText(QString("STATIC XMOS IP: %1\nXMOS port: %2\n").arg(XMOS_IPADRESS).arg(XMOS_PORT));
    msgBox->exec();
    delete msgBox;
    /*
     * //Ethernet box
    box_ethernet        = new QGroupBox;
    buttonPingXMOS      = new QPushButton(tr("PING XMOS"));
    ethernetLayout      = new QGridLayout;
    lineEditXMOSIP      = new QLineEdit;
    buttonPingXMOS->setToolTip(tr("Test if XMOS is available on Network"));
    buttonPingXMOS->setFixedWidth(100);

    lineEditXMOSIP->setToolTip("IP Adress of XMOS box");
    lineEditXMOSIP->setText(QString("IP: %1").arg(XMOS_IPADRESS));
    lineEditXMOSIP->setFixedWidth(120);
    lineEditXMOSIP->setInputMask("IP: 000.000.000.000");
    ethernetLayout->addWidget(buttonPingXMOS,1,0);
    ethernetLayout->addWidget(lineEditXMOSIP,0,0);
    box_ethernet->setToolTip(tr("Ethernet settings"));
    box_ethernet->setTitle(tr("Ethernet"));
    box_ethernet->setMaximumWidth(350);
    box_ethernet->setMaximumHeight(200);
    box_ethernet->setLayout(ethernetLayout);
*/

}

MainWindow::~MainWindow()
{

}


