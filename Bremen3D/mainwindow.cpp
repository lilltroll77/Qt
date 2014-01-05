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
  int type;
  bool link,active;
  double fc, Q ,gain , effect;
  QString displayText;

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
  in >> effect;
  central_widget->main_tab->knob_bremen3D->setValue(effect);


  for(int ch=0 ; ch<(int) channels ; ch++){
      in >> gain; central_widget->dac_tab->channel[ch]->knob->setValue(gain);
      in >> active; central_widget->dac_tab->channel[ch]->muteButton->setChecked(active);
      in >> active; central_widget->dac_tab->channel[ch]->invertButton->setChecked(active);
      in >> displayText ; central_widget->dac_tab->channel[ch]->channelAlias->setText(displayText);
   for(int sec=0 ; sec<(int) sections ; sec++){
        in >> active >> type >> fc >> link >> Q >> gain;
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->B[0];
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->B[1];
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->B[2];
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->A[0];
        in >> central_widget->eq_tab->channel[ch]->eqSection[sec]->A[1];

        central_widget->eq_tab->channel[ch]->eqSection[sec]->groupBox->setChecked(active);
        central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_fc->setValue(fc);
        central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_Q->setValue(Q);
        central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_gain->setValue(gain);
        if(type>=0 || type < ( central_widget->eq_tab->channel[ch]->eqSection[sec]->filterType->maxCount()) )
            central_widget->eq_tab->channel[ch]->eqSection[sec]->filterType->setCurrentIndex( type );
        else{
            msgBoxError.setText(tr("FILE IS CORRUPT"));
            msgBoxError.setWindowTitle(tr("FILE ERROR"));
            msgBoxError.exec();
            return;
        }
    // update freq response data in struct
    //freqz(eqsettings[ch][sec].B  ,eqsettings[ch][sec].A, 44100 ,&f , eqsettings[ch][sec].freq , ejw);
    }
  }
    file.close();
    central_widget->eq_tab->plot->update();
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
  out << (quint32) FILEVERSION;
  //out << QString("Cracy Horse File");
  out << (quint16) CHANNELS;
  out << (quint16) SECTIONS;
  out << central_widget->main_tab->knob_bremen3D->Value();

  for(int ch=0 ; ch<CHANNELS ; ch++){
      out << central_widget->dac_tab->channel[ch]->knob->Value();
      out << central_widget->dac_tab->channel[ch]->muteButton->isChecked();
      out << central_widget->dac_tab->channel[ch]->invertButton->isChecked();
      out << central_widget->dac_tab->channel[ch]->channelAlias->displayText();
   for(int sec=0 ; sec<SECTIONS ; sec++){
        out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->groupBox->isChecked();
        out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->filterType->currentIndex();
        out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_fc->Value();
        out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->link->isChecked();
        out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_Q->Value();
        out <<  central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_gain->Value();
       out << central_widget->eq_tab->channel[ch]->eqSection[sec]->B[0];
       out << central_widget->eq_tab->channel[ch]->eqSection[sec]->B[1];
       out << central_widget->eq_tab->channel[ch]->eqSection[sec]->B[2];
       out << central_widget->eq_tab->channel[ch]->eqSection[sec]->A[0];
       out << central_widget->eq_tab->channel[ch]->eqSection[sec]->A[1];
  }
}

  file.close();
}

MainWindow::~MainWindow()
{

}


