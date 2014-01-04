#include "knob.h"

Knob::Knob(QWidget *parent) :
    QWidget(parent)
{
    widget = new QWidget;
    label_title = new QLabel;
    label_min = new QLabel;
    label_max = new QLabel;
    dial = new QDial();
    spinbox  = new QDoubleSpinBox();
    layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(dial);
    layout->addWidget(spinbox);
    setLayout(layout);
}

void Knob::setMinimum(double min){
   dial->setMinimum(min);
}
