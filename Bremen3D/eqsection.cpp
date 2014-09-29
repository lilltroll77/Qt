#include "eqsection.h"
#include "eqtab.h" // to acess static f and ejw
#include "widget.h"

EQSection::EQSection(QWidget *parent, QCustomPlot *new_plot , Network *udp , Knob *knob_linkedFc_ref):
      QWidget(parent)
  {
      plot = new_plot;
      //knob_linkedFc = knob_linkedFc_ref;
      knob_linkedFc = this->parent()->parent()->findChild<Knob*>("knob_linkedFc");
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
      filterType->addItem(tr("1:st order LP"),QVariant(LP1));
      filterType->addItem(tr("2:nd order LP"),QVariant(LP2));
      filterType->addItem(tr("1:st order HP"),QVariant(HP1));
      filterType->addItem(tr("2:nd order HP"),QVariant(HP2));

      filterType->addItem(tr("Low Shelf"),QVariant(LowShelf));
      filterType->addItem(tr("High Shelf"),QVariant(HighShelf));
      filterType->addItem(tr("Peaking EQ"),QVariant(PeakingEQ));
      filterType->addItem(tr("Notch"),QVariant(Notch));
      filterType->addItem(tr("AllPass"),QVariant(AllPass));
      filterType->addItem(tr("BandPass"),QVariant(BandPass));
      filterType->addItem(tr("Mute"),QVariant(Mute));
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

  bool EQSection::getLinked(){
      return link->isChecked();
  }

  void EQSection::setLinked(bool state , bool blocked){
      if(blocked){
          link->blockSignals(true);
          knob_fc->blockSignals(true);
      }
      link->setChecked(state);
      slot_linkStatusChanged(state);
      if(blocked){
          link->blockSignals(false);
          knob_fc->blockSignals(false);
      }

  }

  double EQSection::getFc(){
      return knob_fc->Value();
  }

  void EQSection::setFc(double Fc , bool blocked){
      if(blocked)
          knob_fc->blockSignals(true);
      if(knob_fc->isEnabled()==true);
        knob_fc->setValue(Fc);
      if(blocked)
          knob_fc->blockSignals(false);
 }

  double EQSection::getQ(){
      return knob_Q->Value();
  }

  void EQSection::setQ(double Q , bool blocked){
      if(blocked)
          knob_Q->blockSignals(true);
      knob_Q->setValue(Q);
      if(blocked)
          knob_Q->blockSignals(false);
 }

  double EQSection::getGain(){
      return knob_gain->Value();
  }

  void EQSection::setGain(double gain , bool blocked){
      if(blocked)
          knob_gain->blockSignals(true);
      knob_gain->setValue(gain);
      if(blocked)
          knob_gain->blockSignals(false);
 }

  bool EQSection::getFilterActive(){
      return groupBox->isChecked();
  }

  void EQSection::setFilterActive(bool state , bool blocked){
      if(blocked)
          groupBox->blockSignals(true);
      groupBox->setChecked(state);
      eqTracer->setVisible(state);
      updateSettingsAndPlot(true); ///can be optimized
      if(blocked)
          groupBox->blockSignals(false);
      }

 filterType_t EQSection::getFilterType(){
       return (filterType_t) filterType->currentIndex();
   }

 void EQSection::setFilterType(filterType_t type ,  bool blocked){
    if(blocked)
        filterType->blockSignals(true);
    filterType->setCurrentIndex((int) type);
    if(blocked)
        filterType->blockSignals(false);
    }

  void::EQSection::updateSettingsAndPlot(bool updatePlot){
      EQ_section_t EQ;
      EQ.Fc = knob_fc->Value();
      EQ.Q =  knob_Q->Value();
      EQ.Gain = knob_gain->Value();
      EQ.type = (filterType_t)  filterType->currentIndex();
      calcFilt( EQ, 44100 , B  ,A );
      freqz(B ,A , freq);
      if(updatePlot)
        emit eqchanged(); //Signal to parent its time to update plot
  }


  //SLOTS
  void EQSection::slot_gainChanged(double gain){
      updateSettingsAndPlot(true);
      float gain_f=(float) gain;
      const char *ptr = (const char *) &gain_f;

      datagram.clear();
      //datagram.append(QString("EQ Gain changed to %1 at EQsection %2 on ch %3").arg(gain).arg(sectionID).arg(channelID));
      datagram[0]= GAIN_CHANGED;
      datagram[1]=(char) channelID;
      datagram[2]=(char) sectionID;
      datagram.insert(4 , ptr , sizeof(float));
      WRITEDATAGRAM
  }

  void EQSection::slot_Q_Changed(double Q){
      updateSettingsAndPlot(true);
      float Q_f=(float) Q;
      const char *ptr = (const char *) &Q_f;
      datagram.clear();
      datagram[0]=Q_CHANGED;
      datagram[1]=(char) channelID;
      datagram[2]=(char) sectionID;
      datagram.insert(4 , ptr , sizeof(float));
      WRITEDATAGRAM

  }

  void EQSection::slot_fcChanged(double fc){
      updateSettingsAndPlot(true);
      eqTracer->setGraphKey(fc);
      float fc_f = (float) fc;
      const char *ptr = (const char *) &fc_f;
      datagram.clear();
      datagram[0]=FC_CHANGED;
      datagram[1]=(char) channelID;
      datagram[2]=(char) sectionID;
      datagram.insert(4 , ptr , sizeof(float));
      WRITEDATAGRAM
  }

  void EQSection::slot_filtertypeChanged(int type){
      if(type == Notch || type == AllPass )
          knob_gain->setDisabled(true);
      else
          knob_gain->setDisabled(false);
      if(type == LP1|| type==HP1)
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
   // qDebug()<< "Link-Fc change to"<<state;
      if(state){
        double fc = knob_linkedFc->Value();
        knob_fc->setValue(fc);
        knob_fc->setDisabled(true);
        //eqTracer->setGraphKey(fc);
        }
    else
        knob_fc->setDisabled(false);
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


