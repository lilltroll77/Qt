#include "eqchannel.h"
EQChannel::EQChannel(QWidget *parent , int new_channel, QCustomPlot *plot_ref ) :
    QWidget(parent){
    plot=plot_ref;
    channel=new_channel;
    scrollArea = new QScrollArea(this);
    pen = new QPen(color , 1 , (Qt::DotLine) );
    scrollArea -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea -> setBackgroundRole(QPalette::Light);
    scrollArea -> setFixedHeight(375);
    scrollArea -> setMaximumWidth(1000);
    scrollArea -> setMinimumWidth(200);

    Hmag.fill(0,PLOTSIZE);
    color.setHsv(32*new_channel,255,255);
    plot->addGraph();
    pen->setColor(color);
    plot->graph(channel)->setPen(*pen);
    plot->graph(channel)->setData( *(f_ref() ), Hmag);
    pen->setStyle(Qt::SolidLine); //update style for eqtracer
    layout_eqsections = new QHBoxLayout;
    for(int section=0 ; section<8 ;section++){
         eqSection[section]=new EQSection(this , plot);
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
    for(int i=0 ; i<PLOTSIZE ; i++)
        Hmag[i] = 20*log10((double) abs(Htot[i])) ;
    plot->graph(channel)->setData( *(f_ref()), Hmag);
    plot->replot();

}