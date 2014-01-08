#include "eqsection.h"
#include "eqtab.h" // to acess static f and ejw
#include "widget.h"

EQSection::EQSection(QWidget *parent, QCustomPlot *new_plot , Network *udp ):
      QWidget(parent)
  {
      plot = new_plot;

      UDP_Socket = udp->UDP_Socket;
      IP_XMOS = udp->IP_XMOS;
      port_XMOS = udp->port_XMOS;

      //Defaults for tracer
      eqTracer = new QCPItemTracer(plot);
      eqTracer->setSize(7);
      eqTracer->setStyle(QCPItemTracer::tsCircle);
      eqTracer->setGraphKey(DEFAULT_FC);
      eqTracer->setInterpolating(true);
      eqTracer->setVisible(false);

      topLayout = new QVBoxLayout;
      layout = new QVBoxLayout;
      groupBox = new QGroupBox(this);
      link = new QCheckBox(tr("Link-Fc") , this );
      link->setToolTip(tr("Link frequency to crossover"));
      link->setChecked(false);

      knob_fc = new Knob(this,logScale);
      knob_fc-> setTitle("Fc [Hz]");
      knob_fc-> setKnobColor("rgb(255, 127, 127)");
      knob_fc->setRange(10,20000,100);
      knob_fc->setDecimals(0);
      knob_fc->setSingleStep(1);
      knob_fc->setValue(DEFAULT_FC);
      knob_Q = new Knob(this, logScale);
      knob_Q-> setTitle("Q");
      knob_Q-> setKnobColor("rgb(127, 255, 127)");
      knob_Q->setRange(0.1,10,100);
      knob_Q->setDecimals(2);
      knob_Q->setSingleStep(0.01);
      knob_Q->setValue(DEFAULT_Q);
      knob_gain = new Knob(this, linScale);
      knob_gain-> setTitle("Gain [dB]");
      knob_gain-> setKnobColor("rgb(127, 127, 255)");
      knob_gain->setRange(-20,10,60);
      knob_gain->setDecimals(1);
      knob_gain->setSingleStep(0.1);
      knob_gain->setValue(DEFAULT_GAIN);
      filterType = new QComboBox(this);
      filterType-> setToolTip(tr("Filter type"));
      filterType->addItem(tr("LowShelf"),QVariant(LowShelf));
      filterType->addItem(tr("HighSelf"),QVariant(HighSelf));
      filterType->addItem(tr("PeakingEQ"),QVariant(PeakingEQ));
      filterType->addItem(tr("Notch"),QVariant(Notch));
      filterType->addItem(tr("AllPass"),QVariant(AllPass));
      filterType->addItem(tr("HighPass"),QVariant(HighPass));
      filterType->addItem(tr("LowPass"),QVariant(LowPass));
      filterType->addItem(tr("BandPass"),QVariant(BandPass));
      filterType->setCurrentIndex(DEFAULT_FILTER);

      layout->addWidget(filterType);
      layout->addWidget(link);
      layout->addWidget(knob_fc);
      layout->addWidget(knob_Q);
      layout->addWidget(knob_gain);
      layout->setSpacing(0);
      layout->setMargin(0);
      groupBox->setLayout(layout);
      groupBox->setMaximumWidth(SETMAXIMUMWIDTH+10);
      groupBox->setCheckable(true);
      groupBox->setChecked(false);
      groupBox->setToolTip(tr("Filter ON/BYPASS"));
      groupBox->setContentsMargins(3,3,3,3);
      connect(link ,      SIGNAL(clicked(bool)) , knob_fc ,       SLOT(setDisabled(bool)) );

      connect(knob_gain , SIGNAL(valueChanged(double)) , this ,   SLOT(slot_gainChanged(double)));
      connect(knob_Q ,    SIGNAL(valueChanged(double)) , this ,   SLOT(slot_Q_Changed(double)));
      connect(knob_fc ,   SIGNAL(valueChanged(double)) , this ,   SLOT(slot_fcChanged(double)));
      connect(filterType, SIGNAL(currentIndexChanged(int)) , this, SLOT(slot_filtertypeChanged(int)) );
      connect(link ,      SIGNAL(clicked(bool))   ,      this ,   SLOT(slot_linkStatusChanged(bool)) );
      connect(groupBox ,  SIGNAL(clicked(bool))   ,      this ,    SLOT(slot_activeEQChanged(bool)) );


      //groupBox->setSizePolicy(QSizePolicy::Minimum);
      topLayout->addWidget(groupBox);
      setLayout(topLayout);
  }


  void EQSection::setSectionID(int newID){
      sectionID=newID;
  }

  void EQSection::setChannelID(int newID){
      channelID=newID;
      eqTracer->setGraph(plot->graph(newID));
      plot->addItem(eqTracer);

  }

  void EQSection::setBoxTitle(const QString & title){
      groupBox->setTitle(title);
  }


  void::EQSection::updateSettingsAndPlot(bool updatePlot){
      double fc = knob_fc->Value();
      double Q =  knob_Q->Value();
      double gain = knob_gain->Value();
      FilterType type = (FilterType)  filterType->currentIndex();
      calcFilt( fc  , Q , gain , 44100 ,type , B  ,A );
      freqz(B ,A , freq);
      if(updatePlot)
        emit eqchanged(); //Signal to parent its time to update plot
  }


  //SLOTS
  void EQSection::slot_gainChanged(double gain){
      updateSettingsAndPlot(true);
      datagram.clear();
      datagram.append(QString("EQ Gain changed to %1 at EQsection %2 on ch %3").arg(gain).arg(sectionID).arg(channelID));
      WRITEDATAGRAM
  }

  void EQSection::slot_Q_Changed(double Q){
      updateSettingsAndPlot(true);
      datagram.clear();
      datagram.append(QString("EQ Q changed to %1 at EQsection %2 on ch %3").arg(Q).arg(sectionID).arg(channelID));
      WRITEDATAGRAM

  }

  void EQSection::slot_fcChanged(double fc){
      eqTracer->setGraphKey(fc);
      updateSettingsAndPlot(true);

      datagram.clear();
      datagram.append(QString("EQ fc changed to %1 at EQsection %2 on ch %3").arg(fc).arg(sectionID).arg(channelID));
      WRITEDATAGRAM
  }

  void EQSection::slot_filtertypeChanged(int type){
      if(type == Notch || type == AllPass )
          knob_gain->setDisabled(true);
      else
          knob_gain->setDisabled(false);

      updateSettingsAndPlot(true);
      datagram.clear();
      datagram.append(QString("Filtertype changed to %1 at EQsection %2 on ch %3").arg(type).arg(sectionID).arg(channelID));
      WRITEDATAGRAM
  }

  void EQSection::slot_linkStatusChanged(bool state){

  }

  void EQSection::slot_activeEQChanged(bool state){
  //
       eqTracer->setVisible(state);
        updateSettingsAndPlot(true); ///can be optimized

       datagram.clear();
       datagram.append(QString("EQsection %2 on ch %3 changed state to %1").arg(state).arg(sectionID).arg(channelID));
       WRITEDATAGRAM

  }
