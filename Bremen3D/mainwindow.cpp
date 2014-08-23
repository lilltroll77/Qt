#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
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
    helpMenu = new QMenu(tr("&Help"), this);
    QMenu *options = new QMenu(tr("&Options"), this);
    //QIcon(":/images/new.png")

    openAction = fileMenu->addAction(tr("&Open ..."));
    connect(openAction,SIGNAL(triggered()),this, SLOT(slot_open()) );

    saveAsAction = fileMenu->addAction(tr("&Save as..."));
    connect(saveAsAction , SIGNAL(triggered()),this,SLOT(slot_saveas()) );

    exitAction = fileMenu->addAction(tr("E&xit"));
    connect(exitAction , SIGNAL(triggered()),this, SLOT( close()) ) ;

    aboutAction= helpMenu->addAction(tr("&About"));
    connect(aboutAction , SIGNAL(triggered()),this, SLOT(slot_about()) );

    graphAction = options->addAction(tr("Graph"));
    connect(graphAction , SIGNAL(triggered()) ,this , SLOT(slot_graph_options()) );

    menubar->addMenu(fileMenu);
    menubar->addMenu(options);
    menubar->addMenu(helpMenu);
    setMenuBar(menubar);

    /*** STAUS BAR ***/

    statusbar = new QStatusBar;
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

void MainWindow::programChanged(int new_program){
     //programSettings[currentProgram].mixer = central_widget->main_tab->knob_bremen3D->Value(); //Store old
    //central_widget->main_tab->knob_bremen3D->setValue( programSettings[new_program].mixer );  //Fetch new
    for(int ch=0 ; ch< CHANNELS ; ch++){
    //DAC Gain
       programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].DACgain = central_widget->dac_tab->channel[ch]->knob->Value(); //Store
       central_widget->dac_tab->channel[ch]->knob->setValue( programSettings[new_program].channel[ch].DACgain  ); // Fetch
    //DAC mute
       programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].mute = central_widget->dac_tab->channel[ch]->muteButton->isChecked();
       central_widget->dac_tab->channel[ch]->muteButton->setChecked( programSettings[new_program].channel[ch].mute );
    //DAC invert
       programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].invert = central_widget->dac_tab->channel[ch]->invertButton->isChecked();
       central_widget->dac_tab->channel[ch]->invertButton->setChecked( programSettings[new_program].channel[ch].invert );
    //DAC Alias
       /// USE pointer instead ???
       programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].alias = central_widget->dac_tab->channel[ch]->channelAlias->displayText();
       central_widget->dac_tab->channel[ch]->channelAlias->setText(  programSettings[new_program].channel[ch].alias );

    for(int sec=0 ; sec < SECTIONS ; sec++){
        ///TELL XMOS ???, but DO NOT REDRAW
        // EQsection active
            programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].eqsection[sec].active = central_widget->eq_tab->channel[ch]->eqSection[sec]->groupBox->isChecked();
            central_widget->eq_tab->channel[ch]->eqSection[sec]->groupBox->blockSignals(true);
            central_widget->eq_tab->channel[ch]->eqSection[sec]->groupBox->setChecked( programSettings[new_program].channel[ch].eqsection[sec].active );
            //update tracer
            central_widget->eq_tab->channel[ch]->eqSection[sec]->eqTracer->setVisible( programSettings[new_program].channel[ch].eqsection[sec].active );
            central_widget->eq_tab->channel[ch]->eqSection[sec]->groupBox->blockSignals(false);

        // EQsection link
            central_widget->eq_tab->channel[ch]->eqSection[sec]->link->blockSignals(true);
            programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].eqsection[sec].link = central_widget->eq_tab->channel[ch]->eqSection[sec]->link->isChecked();
            central_widget->eq_tab->channel[ch]->eqSection[sec]->link->setChecked( programSettings[new_program].channel[ch].eqsection[sec].link );
            central_widget->eq_tab->channel[ch]->eqSection[sec]->link->blockSignals(false);
        // EQsection Q
            central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_Q->blockSignals(true);
            programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].eqsection[sec].Q = central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_Q->Value();
            central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_Q->setValue( programSettings[ new_program ].channel[ch].eqsection[sec].Q   );
            central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_Q->blockSignals(false);
        // EQsection gain
            central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_gain->blockSignals(true);
            programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].eqsection[sec].gain = central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_gain->Value();
            central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_gain->setValue( programSettings[ new_program ].channel[ch].eqsection[sec].gain   );
            central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_gain->blockSignals(false);

         // EQsection filtertype
            programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].eqsection[sec].type = (FilterType) central_widget->eq_tab->channel[ch]->eqSection[sec]->filterType->currentIndex();
            central_widget->eq_tab->channel[ch]->eqSection[sec]->filterType->blockSignals(true);
            central_widget->eq_tab->channel[ch]->eqSection[sec]->filterType->setCurrentIndex( (int) programSettings[ new_program ].channel[ch].eqsection[sec].type );
            central_widget->eq_tab->channel[ch]->eqSection[sec]->filterType->blockSignals(false);

         // EQsection fc will also update eqtracer
             programSettings[/*OLD*/ currentProgram /*PROGRAM*/].channel[ch].eqsection[sec].fc = central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_fc->Value();
             central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_fc->blockSignals(true);
             central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_fc->setValue( programSettings[ new_program ].channel[ch].eqsection[sec].fc   );
             //update eqtracer
             central_widget->eq_tab->channel[ch]->eqSection[sec]->eqTracer->setGraphKey(  programSettings[ new_program ].channel[ch].eqsection[sec].fc );
             central_widget->eq_tab->channel[ch]->eqSection[sec]->knob_fc->blockSignals(false);

             central_widget->eq_tab->channel[ch]->eqSection[sec]->updateSettingsAndPlot(false); //recalc B,A,freq

          }
        central_widget->eq_tab->channel[ch]->recalc_graph(); // All sections updated including individual freq resp ->recalc graph
    }
    currentProgram=new_program;
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
  double fc, Q ,gain , delay ,p,k;
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
  //in >> effect;
  //central_widget->main_tab->knob_bremen3D->setValue(effect);
  in >> p;
  central_widget->mixer_tab->knob_p->setValue(p);

  in >> type;
  if(type<=2)
      central_widget->mixer_tab->radiobuttons_k[type]->setChecked(true);
  in >> k;
      central_widget->mixer_tab->knob_k->setValue(k);

  in >> fc;
  central_widget->eq_tab->knob_linkedFc->setValue(fc);

  for(int ch=0 ; ch<(int) channels ; ch++){
      in >> gain; central_widget->dac_tab->channel[ch]->knob->setValue(gain);
      in >> active; central_widget->dac_tab->channel[ch]->muteButton->setChecked(active);
      in >> active; central_widget->dac_tab->channel[ch]->invertButton->setChecked(active);
      in >> displayText ; central_widget->dac_tab->channel[ch]->channelAlias->setText(displayText);
      in >> delay; central_widget->eq_tab->channel[ch]->knob_delay->setValue(delay);
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
    central_widget->eq_tab->plotMag->update();
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

  out << central_widget->mixer_tab->knob_p->Value();

  bool state=false;
  while(state==false){
    state=central_widget->mixer_tab->radiobuttons_k[i]->isChecked();
    i++;
  }
  out <<i ;
  out << central_widget->mixer_tab->knob_k->Value();

  out << central_widget->eq_tab->knob_linkedFc->Value();

  for(int ch=0 ; ch<CHANNELS ; ch++){
      out << central_widget->dac_tab->channel[ch]->knob->Value();
      out << central_widget->dac_tab->channel[ch]->muteButton->isChecked();
      out << central_widget->dac_tab->channel[ch]->invertButton->isChecked();
      out << central_widget->dac_tab->channel[ch]->channelAlias->displayText();
      out << central_widget->eq_tab->channel[ch]->knob_delay->Value();
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


