#include "eqchannel.h"
#include "defines.h"
#include <QDebug>
EQChannel::EQChannel(QWidget *parent , int new_channel, QCustomPlot *plot_ref, Network *udp , Knob *knob_linkedFc) :
    QWidget(parent){

    plot=plot_ref;
    channel=new_channel;

    UDP_Socket = udp->UDP_Socket;
    IP_XMOS = udp->IP_XMOS;

    scrollArea = new QScrollArea(this);
    pen = new QPen(color , 1 , (Qt::DotLine) );
    scrollArea -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea -> setBackgroundRole(QPalette::Light);
    scrollArea -> setFixedHeight(375);
    scrollArea -> setMaximumWidth(1000);
    scrollArea -> setMinimumWidth(200);

    //   DELAY
    knob_delay  = new Knob(this, linScale);
    knob_delay -> setTitle("t [ms]");
    knob_delay -> setKnobColor("rgb(255, 200, 127)");
    knob_delay -> setRange(0,20,200);
    knob_delay -> setDecimals(2);
    knob_delay -> setSingleStep(0.01);
    knob_delay -> setValue(DEFAULT_DELAY);
    connect(knob_delay , SIGNAL(valueChanged(double)) , this , SLOT(slot_delayChanged(double)) );

    layout_delay = new QHBoxLayout;
    layout_delay->addWidget(knob_delay);
    box_delay = new QGroupBox;
    box_delay->setLayout(layout_delay);
    box_delay->setMaximumWidth(SETMAXIMUMWIDTH+10);
    box_delay->setMaximumHeight(130);
    box_delay->setTitle(tr("Delay"));

    Hmag.fill(0,PLOTSIZE);
    Hdeg.fill(0,PLOTSIZE);
    color.setHsv(32*new_channel,255,255);
    plot->addGraph();
    pen->setColor(color);
    plot->graph(channel)->setPen(*pen);
    plot->graph(channel)->setData( *(f_ref() ), Hmag);
    pen->setStyle(Qt::SolidLine); //update style for eqtracer
    layout_eqsections = new QHBoxLayout;
    layout_eqsections->addWidget(box_delay);

    for(int section=0 ; section<8 ;section++){
         eqSection[section]=new EQSection(this , plot , udp , knob_linkedFc);
         eqSection[section]->setBoxTitle(QString("Filter %1").arg(section));
         eqSection[section]->setSectionID(section);
         eqSection[section]->setChannelID(new_channel);
         eqSection[section]->setFixedHeight(350);
         eqSection[section]->eqTracer->setPen(*pen);
         layout_eqsections->addWidget(eqSection[section]);
         connect(eqSection[section],SIGNAL(eqchanged()) , this , SLOT(recalc_graph() ) );
     }
     QWidget *widget = new QWidget;
     widget->setLayout(layout_eqsections);
     //Adds widget as a scroll bar widget
     scrollArea-> addScrollBarWidget( widget, Qt::AlignTop);
     //Sets the scroll area's widget.
     //Note that You must add the layout of widget before you call this function
     scrollArea->setWidget(widget);

     QGridLayout *layout = new QGridLayout(this);
     label_link = new QLabel(tr("Link to Ch: "));
     QSignalMapper *mapper = new QSignalMapper;
     label_link->setToolTip(tr("Link the EQ settings of this channel to other channels"));
     layout->addWidget(scrollArea,0,0,1,10);
     layout->addWidget(label_link,1,0);
     for(int ch=0 ; ch<CHANNELS ; ch++){
        linkChannel[ch] =new QCheckBox(QString("%1").arg(ch) , this);
        layout->addWidget(linkChannel[ch],1,1+ch);
        mapper->setMapping(linkChannel[ch] , channel*CHANNELS + ch  );
        connect(linkChannel[ch] , SIGNAL(toggled(bool)) , mapper ,SLOT( map() ));
         }
     connect(mapper , SIGNAL(mapped(int)) , parent , SLOT(slot_linkchannel(int)) );
     linkChannel[channel]->setDisabled(true);

     setLayout(layout);


 }

void EQChannel::enableGraph(){
    pen->setWidth(2);
    pen->setStyle(Qt::SolidLine);
    plot->graph(channel)->setPen(*pen);
    for(int sec=0 ; sec < SECTIONS ; sec++)
        eqSection[sec]->eqTracer->setPen(*pen);
    plot->replot();
}

void EQChannel::disableGraph(){
    pen->setWidth(1);
    pen->setStyle(Qt::DotLine);
    plot->graph(channel)->setPen(*pen);
    for(int sec=0 ; sec < SECTIONS ; sec++)
        eqSection[sec]->eqTracer->setPen(*pen);
    plot->replot();
}

void EQChannel::slot_delayChanged(double delay){
    float delay_fixed=(uint) round(1000*delay);
    const char *ptr = (const char *) &delay_fixed;
    datagram.clear();
    datagram[0]=DELAY_CHANGED;
    datagram[1]=channel;
    datagram.insert(4 , ptr , sizeof(uint));
    //qDebug() << datagram;
    WRITEDATAGRAM;
}

void EQChannel::recalc_graph(){
    bool state;
    for(int i=0 ; i<PLOTSIZE ; i++)
        Htot[i]=1;
    for(int section=0 ; section<SECTIONS ; section++){
        state = eqSection[section]->groupBox->isChecked();
        if( state )
            for(int i=0 ; i<PLOTSIZE ; i++)
                Htot[i] *= eqSection[section]->freq[i];
    }
    for(int i=0 ; i<PLOTSIZE ; i++){
        Hmag[i] = 20*log10((double) abs(Htot[i])) ;
        Hdeg[i] = 180 / M_PI * arg (Htot[i]);
    }
    plot->graph(channel)->setData( *(f_ref()), Hmag);
    //plot->graph(channel)->setData( *(f_ref()), Hdeg);
    plot->replot();

}
