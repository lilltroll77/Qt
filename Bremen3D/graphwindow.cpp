
#include "graphwindow.h"

GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent)
{
setWindowTitle("Frequency response");
setGeometry(400, 250, 542, 390);
name = "Quadratic Demo";
plot = new QCustomPlot;

// generate some data:
#define SIZE 800
QVector<double> f(SIZE), h0(SIZE) ,h1(SIZE) ; // initialize with entries 0..100
QVector<double> fTicks(9);
double fs=44100;
fTicks << 20 << 50 << 100;
for (int i=0; i<800; ++i)
{
  f[i] = 20*pow(10 ,3*(double) i / SIZE);
  h0[i] = 3*sin(f[i]/SIZE);  // let's plot a quadratic function
  h1[i] = - 4*sin(f[i]/SIZE);
}
// create graph and assign data to it:
plot->addGraph();
plot->addGraph();
plot->graph(1)->setData(f, h1);
// give the axes some labels


QPen *pen0 =  new QPen;
QPen *pen1 =  new QPen;

pen0->setColor(QColor(255,0,0));
pen0->setWidth(3);
pen1->setColor(QColor(0,255,0));
pen1->setWidth(3);

plot->graph(0)->setPen(*pen0);
plot->graph(1)->setPen(*pen1);

plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
plot->xAxis->setTickVector(fTicks);
plot->xAxis->setAutoSubTicks(false);
plot->xAxis->setSubTickCount(9);
plot->xAxis->setLabel("Frequeny [Hz]");
plot->yAxis->setLabel("Gain [dB]");
// set axes ranges, so we see all data:
plot->xAxis->setRange(20, 20000);
plot->yAxis->setRange(-20, 10);

plot->yAxis->setAutoTickStep(false);
plot->yAxis->setAutoSubTicks(false);
plot->yAxis->setTickStep(5);
plot->yAxis->setSubTickCount(4);

plot->replot();
/*
QCPItemLine *line = new QCPItemLine;

QCPAbstractItem *  item=new QCPAbstractItem;
item->
plot->addItem(item);
*/
//plot->replot();

setCentralWidget(plot);
statusbar=new QStatusBar(this);
setStatusBar(statusbar);

connect(plot , SIGNAL( mouseMove(QMouseEvent*)) , this , SLOT(updateStatusBar(QMouseEvent*)) );

}

void GraphWindow::updateStatusBar(QMouseEvent* mouse){
    int x_pos= mouse->globalX() -x();
    int y_pos= mouse->globalY() - y();
    //Transform::lin2log(x_pos , y_pos);
    statusbar->showMessage( QString("X=%1, Y=%2").arg(x_pos).arg(y_pos));
}
