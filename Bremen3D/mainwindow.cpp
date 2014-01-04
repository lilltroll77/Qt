#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mainwindow = parent;

    msgBoxAbout = new QMessageBox;
    msgBoxAbout->setText(QString("XMOS-GUI ver: %1\n\Written in C++\nBy Mikael Bohman 2013").arg(0.01));
    msgBoxAbout->setFixedWidth(640);
    msgBoxAbout->setFixedHeight(480);
    msgBoxAbout->setWindowTitle(tr("About"));



/*** MENU BAR ***/
    menubar = new QMenuBar(this);
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->setObjectName("fileMenu");
    helpMenu = new QMenu(tr("&Help"), this);
    //QIcon(":/images/new.png")

    openAction = fileMenu->addAction(tr("&Open File..."));
    connect(openAction,SIGNAL(triggered()),this,SLOT(slot_open()) );

    saveAsAction = fileMenu->addAction(tr("&Save as..."));
    connect(saveAsAction , SIGNAL(triggered()),this,SLOT(slot_saveas()) );

    exitAction = fileMenu->addAction(tr("E&xit"));


    aboutAction= helpMenu->addAction(tr("&About"));
    connect(aboutAction , SIGNAL(triggered()),this,SLOT(slot_about()) );


    menubar->addMenu(fileMenu);
    menubar->addMenu(helpMenu);
    setMenuBar(menubar);

    /*** STAUS BAR ***/

        statusbar->setObjectName("statusbar");
        statusbar->showMessage(tr("Not connected"));
        setStatusBar(statusbar);
        setWindowTitle("Alfa release");


    /*** CENTRAL WIDGET ***/

    central_widget=new Widget(this , statusbar);
    setCentralWidget(central_widget);
    central_widget->setObjectName("central_widget");
   // connect(central_widget-> , SIGNAL() , this ,SLOT() );

/*** TOOLBARS ***/
    fileToolBar = addToolBar(tr("Plot"));
    plotAction=fileToolBar->addAction("Plot frequency response");


   }

void MainWindow::slot_about(){
   msgBoxAbout->exec();
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
  quint32 data;
  double fc, Q ,gain;

  in >> magic;
  if(magic != MAGIC_FILECHECKNUMBER){
      msgBoxError.setText(tr("BAD FILE FORMAT"));
      msgBoxError.setWindowTitle(tr("FILE ERROR"));
      msgBoxError.exec();
      return;
}
  in >> version;
  if(version<QDataStream::Qt_5_2){
      msgBoxError.setText(tr("FILE TOO OLD"));
      msgBoxError.setWindowTitle(tr("FILE ERROR"));
      msgBoxError.exec();
      return;
  }
  in >> str >>channels >> sections;
  for(int ch=0 ; ch<(int) channels ; ch++)
   for(int sec=0 ; sec<(int) sections ; sec++){
    in >> fc;
    eqsettings[ch][sec].fc = fc;
    eq_tab->channel[ch]->eqSection[sec]->knob_fc->setValue(fc);
    in >> Q;
    eqsettings[ch][sec].Q=Q;
    eq_tab->channel[ch]->eqSection[sec]->knob_Q->setValue(Q);
    in >> gain;
    eqsettings[ch][sec].gain =gain;
    eq_tab->channel[ch]->eqSection[sec]->knob_gain->setValue(gain);
    in >> eqsettings[ch][sec].active;
    in >> data;
    eqsettings[ch][sec].filtertype = (FilterType) data;
    in >> eqsettings[ch][sec].B[0];
    in >> eqsettings[ch][sec].B[1];
    in >> eqsettings[ch][sec].B[2];
    in >> eqsettings[ch][sec].A[0];
    in >> eqsettings[ch][sec].A[1];
    // update freq response data in struct
    //freqz(eqsettings[ch][sec].B  ,eqsettings[ch][sec].A, 44100 ,&f , eqsettings[ch][sec].freq , ejw);

   }
    file.close();
    plot->update();


}


void MainWindow::slot_saveas(){
  QString fileName;
  QFileDialog *filedialog=new QFileDialog;
  filedialog->setDefaultSuffix("DSPsettings");
  fileName=filedialog->getSaveFileName(this , tr("Save File"),"","DSPsettings");
  QFile file(fileName);
  file.open(QIODevice::WriteOnly);
  QDataStream out(&file);
  out << (quint64) MAGIC_FILECHECKNUMBER;
  out.setVersion(QDataStream::Qt_5_2);
  out << QString("Cracy Horse File");
  out << (quint16) CHANNELS;
  out << (quint16) SECTIONS;
  for(int ch=0 ; ch<CHANNELS ; ch++)
   for(int sec=0 ; sec<SECTIONS ; sec++){
    //fetch from knobs ??
    out << eqsettings[ch][sec].fc;
    out << eqsettings[ch][sec].Q;
    out << eqsettings[ch][sec].gain;
    out << eqsettings[ch][sec].active;
    out << (quint32) eqsettings[ch][sec].filtertype;
    out << eqsettings[ch][sec].B[0];
    out << eqsettings[ch][sec].B[1];
    out << eqsettings[ch][sec].B[2];
    out << eqsettings[ch][sec].A[0];
    out << eqsettings[ch][sec].A[1];
  }

  file.close();
}

MainWindow::~MainWindow()
{

}


