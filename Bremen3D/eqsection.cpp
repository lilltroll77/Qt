#include "eqsection.h"
#include "eqtab.h" // to acess static f and ejw
#include "widget.h"

EQSection::EQSection(QWidget *parent, QCustomPlot *new_plot , Network *udp , Knob *knob_linkedFc_ref):
      QWidget(parent)
  {
      plot = new_plot;
      knob_linkedFc = knob_linkedFc_ref;

      UDP_Socket    = udp->UDP_Socket;
      IP_XMOS       = udp->IP_XMOS;
      port_XMOS     = udp->port_XMOS;

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
      filterType->addItem(tr("LowPass 1:st"),QVariant(LowPass1));
      filterType->addItem(tr("LowPass 2:nd"),QVariant(LowPass));
      filterType->addItem(tr("HighPass 1:st"),QVariant(HighPass1));
      filterType->addItem(tr("HighPass 2:st"),QVariant(HighPass));
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
      uint mGain=(int) round(1000*gain);
      const char *ptr = (const char *) &mGain;

      datagram.clear();
      //datagram.append(QString("EQ Gain changed to %1 at EQsection %2 on ch %3").arg(gain).arg(sectionID).arg(channelID));
      datagram[0]= GAIN_CHANGED;
      datagram[1]=(char) channelID;
      datagram[2]=(char) sectionID;
      datagram.insert(4 , ptr , sizeof(int));
      WRITEDATAGRAM
  }

  void EQSection::slot_Q_Changed(double Q){
      updateSettingsAndPlot(true);
      uint mQ=(uint) round(1000*Q);
      const char *ptr = (const char *) &mQ;
      datagram.clear();
      datagram[0]=Q_CHANGED;
      datagram[1]=(char) channelID;
      datagram[2]=(char) sectionID;
      datagram.insert(4 , ptr , sizeof(uint));
      WRITEDATAGRAM

  }

  void EQSection::slot_fcChanged(double fc){
      updateSettingsAndPlot(true);
      eqTracer->setGraphKey(fc);
      uint mfc=(uint) round(1000*fc);
      const char *ptr = (const char *) &mfc;
      datagram.clear();
      datagram[0]=FC_CHANGED;
      datagram[1]=(char) channelID;
      datagram[2]=(char) sectionID;
      datagram.insert(4 , ptr , sizeof(uint));
      WRITEDATAGRAM
  }

  void EQSection::slot_filtertypeChanged(int type){
      if(type == Notch || type == AllPass )
          knob_gain->setDisabled(true);
      else
          knob_gain->setDisabled(false);
      if(type == LowPass1|| type==HighPass1)
          knob_Q->setDisabled(true);
      else
          knob_Q->setDisabled(false);

      updateSettingsAndPlot(true);
      datagram.clear();
      datagram[0]=FILTERTYPE_CHANGED;
      datagram[1]=(char) channelID;
      datagram[2]=(char) sectionID;
      datagram[3]=(char) type;

      WRITEDATAGRAM
  }

  void EQSection::slot_linkStatusChanged(bool state){
    if(state)
      knob_fc->setValue(knob_linkedFc->Value()  );
    }

  void EQSection::slot_activeEQChanged(bool state){
  //
       eqTracer->setVisible(state);
        updateSettingsAndPlot(true); ///can be optimized

       datagram.clear();
       datagram[0]=EQ_ACTIVE_CHANGED;
       datagram[1]=(char) channelID;
       datagram[2]=(char) sectionID;
       datagram[3]=(char) state;
       WRITEDATAGRAM

  }
