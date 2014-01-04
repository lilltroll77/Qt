#include "eqtab.h"
#include "widget.h"

EQSection::EQSection(QWidget *parent) :
    QWidget(parent)
{
   // eqsettings_pointer = &eqsettings[0][0];
    topLayout = new QVBoxLayout;
    layout = new QVBoxLayout;
    groupBox = new QGroupBox;
    link = new QCheckBox(tr("Link-Fc"));
    link->setToolTip(tr("Link frequency to crossover"));
    link->setChecked(false);

    knob_fc = new Knob(logScale);
    knob_fc-> setTitle("Fc [Hz]");
    knob_fc-> setKnobColor("rgb(255, 127, 127)");
    knob_fc->setRange(10,20000,100);
    knob_fc->setDecimals(0);
    knob_fc->setSingleStep(1);
    knob_fc->setValue(DEFAULT_FC);
    knob_Q = new Knob(logScale);
    knob_Q-> setTitle("Q");
    knob_Q-> setKnobColor("rgb(127, 255, 127)");
    knob_Q->setRange(0.1,10,100);
    knob_Q->setDecimals(2);
    knob_Q->setSingleStep(0.01);
    knob_Q->setValue(DEFAULT_Q);
    knob_gain = new Knob(linScale);
    knob_gain-> setTitle("Gain [dB]");
    knob_gain-> setKnobColor("rgb(127, 127, 255)");
    knob_gain->setRange(-20,10,60);
    knob_gain->setDecimals(1);
    knob_gain->setSingleStep(0.1);
    knob_gain->setValue(DEFAULT_GAIN);
    filterType = new QComboBox;
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

void EQSection::setPointer2EQSettings(EQsettings *new_eqSettings){
    eqSettings = new_eqSettings;
}

void EQSection::setSectionID(int newID){
    sectionID=newID;
}


void EQSection::setChannelID(int newID){
    channelID=newID;
}


void EQSection::setBoxTitle(const QString & title){
    groupBox->setTitle(title);
}


void::EQSection::updateSettingsAndPlot(){

    bool state;
    double fc = eqsettings[channelID][sectionID].fc;
    double Q = eqsettings[channelID][sectionID].Q;
    double gain = eqsettings[channelID][sectionID].gain;
    FilterType type = eqsettings[channelID][sectionID].filtertype;
    calcFilt( fc  , Q , gain , 44100 ,type , B  ,A );
    freqz(B  ,A, 44100 ,&f , eqsettings[channelID][sectionID].freq , ejw);
    for(int f=0 ; f<PLOTSIZE ; f++){
        //reset vector to unity
        Htot[f].imag()=0;
        Htot[f].real()=1;
        for(int section=0 ; section<SECTIONS ; section++){
            state = eqsettings[channelID][section].active;
            if( state )
                Htot[f] *= eqsettings[channelID][section].freq[f];
        }
        H[channelID][f] = 20*log10((double) abs(Htot[f])) ;
    }
    plot->graph(channelID)->setData(f, H[channelID]);
    plot->replot();
}


//SLOTS
void EQSection::slot_gainChanged(double gain){
    eqsettings[channelID][sectionID].gain = gain;
    updateSettingsAndPlot();
    //eqSettings->gain[channelID][sectionID]=gain; //Update gain
    datagram.clear();
    datagram.append(QString("EQ Gain changed to %1 at EQsection %2 on ch %3").arg(gain).arg(sectionID).arg(channelID));
    UDP_Socket->writeDatagram(datagram.data(), datagram.size(), *IP_XMOS,  XMOS_PORT);
}

void EQSection::slot_Q_Changed(double Q){
    eqsettings[channelID][sectionID].Q = Q;
    updateSettingsAndPlot();
    datagram.clear();
    datagram.append(QString("EQ Q changed to %1 at EQsection %2 on ch %3").arg(Q).arg(sectionID).arg(channelID));
    UDP_Socket->writeDatagram(datagram.data(), datagram.size(), *IP_XMOS,  XMOS_PORT);

}

void EQSection::slot_fcChanged(double fc){
    eqTracer[channelID][sectionID]->setGraphKey(fc);
    eqsettings[channelID][sectionID].fc = fc;
    updateSettingsAndPlot();

    datagram.clear();
    datagram.append(QString("EQ fc changed to %1 at EQsection %2 on ch %3").arg(fc).arg(sectionID).arg(channelID));
    UDP_Socket->writeDatagram(datagram.data(), datagram.size(), *IP_XMOS,  XMOS_PORT);
}

void EQSection::slot_filtertypeChanged(int type){
    if(type == Notch || type == AllPass )
        knob_gain->setDisabled(true);
    else
        knob_gain->setDisabled(false);

    eqsettings[channelID][sectionID].filtertype = (FilterType)type;
    updateSettingsAndPlot();
    datagram.clear();
    datagram.append(QString("Filtertype changed to %1 at EQsection %2 on ch %3").arg(type).arg(sectionID).arg(channelID));
    UDP_Socket->writeDatagram(datagram.data(), datagram.size(), *IP_XMOS,  XMOS_PORT);
}

void EQSection::slot_linkStatusChanged(bool state){

}

void EQSection::slot_activeEQChanged(bool state){
     eqsettings[channelID][sectionID].active=(int)state;
     eqTracer[channelID][sectionID]->setVisible(state);
     updateSettingsAndPlot();

     datagram.clear();
     datagram.append(QString("EQsection %2 on ch %3 changed state to %1").arg(state).arg(sectionID).arg(channelID));
     UDP_Socket->writeDatagram(datagram.data(), datagram.size(), *IP_XMOS,  XMOS_PORT);

}

// *******************************************************************************************

EQTab::EQTab(QWidget *parent) :
    QWidget(parent)
{

    channelTabs = new QTabWidget(parent);
    layout = new QGridLayout;
    plot = new QCustomPlot;


    for(int i=0 ; i<8 ;i++){
        area[i] = new QScrollArea;
        area[i]->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        //color.setHsv(32*i,255,255);

        //area[i]->setStyleSheet(QString("Background-Color: red" ));
        channel[i] = new EQChannel(this , i);
        area[i]->addScrollBarWidget(channel[i],Qt::AlignBottom);
        area[i]->setWidget(channel[i]);
        channelTabs->addTab(area[i] , (QString("%1").arg(i)) );

        area[i]->show();
        channelTabs->setToolTip(tr("Channel Selector"));
        connect(channelTabs , SIGNAL(currentChanged(int)) , this , SLOT(slot_updatePlot(int)));
    }
    //QPainter *painter = new QPainter(this);
    //painter->drawEllipse(8,8,16,16);
    //painter->setBrush(Qt::red);
    //QIcon *icon = new QIcon;
    //icon->paint(painter , 0,0,16,16);
    //channelTabs->setTabIcon(0,*icon);
    channelTabs->setTabPosition(QTabWidget::West);
    layout->addWidget(channelTabs,0,0);
    layout->setContentsMargins(3,3,3,3);

    f.resize(PLOTSIZE);
    H.resize(8);
    for(int ch=0; ch<8 ; ch++)
        H[ch].fill(0,PLOTSIZE);

    std::complex<double> jw;

    double S =log10(FS/2) / log10(FMIN);
    for (int i=0; i<PLOTSIZE; ++i){
        f[i] = FMIN *pow(10 ,S*(double) i / PLOTSIZE);
        jw.imag() = 2* M_PI * f[i] / FS;
        ejw[i]=exp(jw); // Precalc e^jw and where w = 2*pi f/fs
        }

for(int ch=0; ch<8 ; ch++){
        color.setHsv(32*ch,255,255);
        pen[ch] = new QPen(color , 1 , (Qt::DotLine) );

        plot->addGraph();
        plot->graph(ch)->setData(f, H[ch]);
        plot->graph(ch)->setPen(*pen[ch]);
        channelTabs->createWinId();
        for(int section=0 ; section < SECTIONS ; section++){
            eqTracer[ch][section]= new QCPItemTracer(plot);
            eqTracer[ch][section]->setGraph(plot->graph(ch));
            eqTracer[ch][section]->setSize(7);
            eqTracer[ch][section]->setStyle(QCPItemTracer::tsCircle);
            eqTracer[ch][section]->setPen(color);
            eqTracer[ch][section]->setGraphKey(DEFAULT_FC);
            eqTracer[ch][section]->setVisible(false);
            eqTracer[ch][section]->setInterpolating(true);
            plot->addItem(eqTracer[ch][section]);

            //connect(plot , SIGNAL(itemClick(QCPAbstractItem*,QMouseEvent*)) , this , SLOT(updateKnobs()));
            //setBrush(Qt::red);
        }
}

    pen[0]->setWidth(2);
    pen[0]->setStyle(Qt::SolidLine);
    plot->graph(0)->setPen(*pen[0]);


    //plot->setMinimumWidth(500);
    plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
    plot->xAxis->setAutoSubTicks(false);
    plot->xAxis->setSubTickCount(9);
    plot->xAxis->setLabel("Frequeny [Hz]");

    plot->yAxis->setLabel("Gain [dB]");
    plot->xAxis->setRange(FMIN , FS/2);
    plot->yAxis->setRange(-20, 10);

    plot->yAxis->setAutoTickStep(false);
    plot->yAxis->setAutoSubTicks(false);
    plot->yAxis->setTickStep(5);
    plot->yAxis->setSubTickCount(4);
    plot->replot();

    layout->addWidget(plot,0,1);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,1);
    setLayout(layout);
}

void EQTab::slot_updatePlot(int newChannel){
    pen[activeChannel]->setWidth(1);
    pen[activeChannel]->setStyle(Qt::DotLine);
    plot->graph(activeChannel)->setPen(*pen[activeChannel]);
    activeChannel=newChannel;
    pen[activeChannel]->setWidth(2);
    pen[activeChannel]->setStyle(Qt::SolidLine);
    plot->graph(activeChannel)->setPen(*pen[activeChannel]);
    plot->replot();
    //channelTabs->
}

// **************************************************************

EQChannel::EQChannel(QWidget *parent , int channel) :
    QWidget(parent){
    layout_eqsections = new QHBoxLayout;
    for(int section=0 ; section<8 ;section++){
         eqSection[section]=new EQSection;
         eqSection[section]->setBoxTitle(QString("Filter %1").arg(section));
         eqSection[section]->setSectionID(section);
         eqSection[section]->setChannelID(channel);
         eqSection[section]->setFixedHeight(350);
         layout_eqsections->addWidget(eqSection[section]);
     }
     setLayout(layout_eqsections);
}
/*


     //   DELAY
     knob_delay  = new Knob(linScale);
     knob_delay -> setTitle("t [ms]");
     knob_delay -> setKnobColor("rgb(255, 200, 127)");
     knob_delay -> setRange(0,20,100);
     knob_delay -> setDecimals(3);
     knob_delay -> setSingleStep(0.001);
    groupBoxDelay  = new QGroupBox;
     layout_delay   = new QVBoxLayout;
     layout_delay->addWidget(knob_delay);
     groupBoxDelay->setLayout(layout_delay);
     groupBoxDelay->setMaximumWidth(SETMAXIMUMWIDTH+10);
     groupBoxDelay->setMaximumHeight(180);
     groupBoxDelay->setTitle(tr("Delay"));
     layout->addWidget(groupBoxDelay,0,0);

 //SIGNAL
     connect(channelSelector , SIGNAL(currentIndexChanged(int)) , this , SLOT(updateEQchannel(int)) );
     connect(knob_delay , SIGNAL(valueChanged(double)) , this , SLOT(slot_send_Delay(double)) );
     setLayout(layout);
}

void EQChannel::updateEQchannel(int new_channel){
    knob_delay->blockSignals(true);
    knob_delay->setValue(delay[new_channel]); // update  knob to new value without telling XMOS
    knob_delay->blockSignals(false);

    for(int section=0 ; section<8 ;section++){
         eqSection[section]->groupBox->blockSignals(true);
         bool active =eqSettings.active[new_channel][section];
         eqSection[section]->groupBox->setChecked(active);
         eqSection[section]->groupBox->blockSignals(false);

         eqSection[section]->filterType->blockSignals(true);
         int type = (int) eqSettings.filtertype[new_channel][section];
         eqSection[section]->filterType->setCurrentIndex(type);
         eqSection[section]->filterType->blockSignals(false);

         bool state = eqSettings.link[new_channel][section];
         eqSection[section]->link->setChecked( state );

         eqSection[section]->knob_fc->blockSignals(true);
         double fc = eqSettings.fc[new_channel][section] ;
         eqSection[section]->knob_fc->setValue( fc );
         eqSection[section]->knob_fc->blockSignals(false);

         eqSection[section]->knob_Q->blockSignals(true);
         double Q = eqSettings.Q[new_channel][section];
         eqSection[section]->knob_Q->setValue(Q);
         eqSection[section]->knob_Q->blockSignals(false);

         eqSection[section]->knob_gain->blockSignals(true);
         double gain = eqSettings.gain[new_channel][section] ;
         eqSection[section]->knob_gain->setValue(gain);
         eqSection[section]->knob_gain->blockSignals(false);

         eqSection[section]->setChannelID(new_channel);
    }
    current_channel=new_channel;
}

int EQChannel::getCurrentChannel(){
    return current_channel;
}

void EQChannel::slot_send_Delay(double new_delay){
    delay[current_channel]=new_delay;
    datagram.clear();
    datagram.append( QString("DELAY CHANGED on Channel %1 to %2 ms").arg(current_channel).arg(new_delay));
    udpSocket->writeDatagram(datagram.data(), datagram.size(), *IP,  XMOS_PORT);
}

void EQChannel::setIP(QHostAddress *newIP){
    IP = newIP;
    for(int section=0 ; section<8 ;section++)
        eqSection[section]->setIP(IP);
}

void EQChannel::setPort(quint16 *new_port){
    port = new_port;
    for(int section=0 ; section<8 ;section++)
        eqSection[section]->setPort(port);
}
void EQChannel::setUDPsocket(QUdpSocket *new_udpSocket){
    udpSocket = new_udpSocket;
     for(int section=0 ; section<8 ;section++)
        eqSection[section]->setUDPsocket(udpSocket);
}

void EQChannel::setStatusBar(QStatusBar *new_statusbar){
    statusbar = new_statusbar;
}

*/
