#include "knob.h"

void updateDialPosition(QDial *dial , QDoubleSpinBox *spinbox , Scale scale){
    double minSpin=spinbox->minimum();
    double maxSpin=spinbox->maximum();
    double maxDial=(double) dial->maximum();
    double valueSpin=spinbox->value();
    double norm;

    if(scale==logScale){
        //map f into [0 1] with logscale
        //( log10(x)/log10(min)-1 ) / ( log10(max)/log10(min)-1 )
        double A= log10(minSpin);
        double B= log10(maxSpin)/log10(minSpin) -1;
        norm =(log10(valueSpin)/ A-1 ) / B;
    }else{
        //Normalize the Spinbox value to a value [0 1]
        // y =kx+m -> x= (y-m)/k
        norm = (valueSpin - minSpin) / (maxSpin-minSpin )  ;
    }

    dial->setSliderPosition((int) round( maxDial * norm));
}

void updateSpinboxValue( QDial *dial , QDoubleSpinBox *spinbox , Scale scale){
    double minSpin=spinbox->minimum();
    double maxSpin=spinbox->maximum();
    double maxDial=(double) dial->maximum();
    double valueDial=(double) dial->value();
    //Normalize the dial value to a value [0 1]
    double norm = (double) valueDial/ maxDial;
    double value;
    if(scale==logScale){
        double A= log10(minSpin);
        double B= log10(maxSpin)/log10(minSpin) -1;
        value = ( pow(10 , A * (1 + B * norm)) );
    }
    else
        value = ( (maxSpin-minSpin)*norm + minSpin );
    // disable the spinbox to fire a event that the spinbox has changed
     spinbox->setValue(value);
}


Knob::Knob(QWidget *parent ,const Scale scaletype) :
    QWidget(parent)
{
    {
        widget = new QWidget;
        label_title = new QLabel(tr("title"),this);
        //lineEdit_min = new QLineEdit();
        //lineEdit_max = new QLineEdit();
//        QString style =QString("background-color: rgb(220, 200, 200)");
//        lineEdit_min->setMaximumWidth(30);
//        lineEdit_max->setMaximumWidth(30);
//        lineEdit_min->setFrame(QFrame::NoFrame);
//        lineEdit_max->setFrame(QFrame::NoFrame);
//        lineEdit_min->setStyleSheet(style);
//        lineEdit_max->setStyleSheet(style);
//        lineEdit_min->setToolTip(tr("Minimum Value"));
//        lineEdit_max->setToolTip(tr("Maximum Value"));

        dial = new QDial(this);
        dial->setNotchesVisible(true);
        //dial->setTracking(false);
        dial->setMaximumWidth(300);
        dial->setMaximumHeight(300);
        //dial->setMouseTracking();
        scale=scaletype;
        spinbox  = new QDoubleSpinBox(this);
        spinbox->setAlignment(Qt::AlignRight);
        layout = new QVBoxLayout;
        layout->setSpacing(2);
        layout->setContentsMargins(1,1,1,5);
        layout->addWidget(label_title , 0 ,Qt::AlignHCenter);
        layout->addWidget(dial,Qt::AlignHCenter);
        layout->addWidget(spinbox,Qt::AlignHCenter);
        //layout->addWidget(lineEdit_min,2,0, Qt::AlignBottom | Qt::AlignLeft);
        //layout->addWidget(lineEdit_max,2,0, Qt::AlignBottom | Qt::AlignRight);
        setLayout(layout);

        connect(dial,   SIGNAL(valueChanged(int))      , this , SLOT(dial_changed(int)) );
        connect(spinbox , SIGNAL(valueChanged(double)) , this , SLOT(spinbox_changed()) );
    }
}
//SLOTS
void Knob::dial_changed(int value){
    spinbox->blockSignals(true);
    updateSpinboxValue(dial , spinbox , scale);
    spinbox->blockSignals(false);
    emit valueChanged(spinbox->value() );
    //Only update if dial is pressed,
  /*  int mid = dial->maximum() >>1;
    int max = dial->maximum();
    int min = dial->minimum();
    if(!atMax & !atMin){
        dial->setValue(value);
        updateSpinboxValue(dial , spinbox , scale);
    }
     if(value == max)
        atMax=true;
     if(value == min )
        atMin=true;

     if(atMax){
         if(value<mid)
            dial->setSliderPosition(max);
         else{
            atMax=false;
            dial->setValue(value);
            updateSpinboxValue(dial , spinbox , scale);
      }}
      if(atMin){
         if(value>mid )
             dial->setSliderPosition(min);
         else{
            atMin=false;
            dial->setValue(value);
            updateSpinboxValue(dial , spinbox , scale);
         }
      }*/
}

void Knob::spinbox_changed(){
    dial->blockSignals(true);
    updateDialPosition(dial , spinbox , scale);
    emit valueChanged(spinbox->value() );
    dial->blockSignals(false);
}


void updateWidth(QDoubleSpinBox *spinbox){
    const int charWidth=8;
    double min = spinbox->minimum();
    double max = spinbox->maximum();
    int dec = spinbox->decimals();

    QString string1 = QString("%1").arg(min,1);
    QString string2 = QString("%1").arg(max,1);

    int len1 = string1.length();
    int len2 = string2.length();
    int pixels=15; //up/down arrow

    if (len1> len2)
        pixels+= charWidth*len1;
    else
        pixels+= charWidth*len2;

    if ( dec > 0)
        pixels+=charWidth*dec+charWidth;  //add room for decimals
    else
        pixels+=charWidth;
    if ( min<0)
        pixels+=charWidth; //add rom for "-"
    spinbox->setFixedWidth(pixels);
}

void Knob::setRange(double min , double max , int steps){
    spinbox->setRange(min,max);
    updateWidth(spinbox);
    dial->setRange(0,steps);

    updateDialPosition ( dial , spinbox , scale);
}

void Knob::setValue(double arg){
   spinbox->setValue(arg);
   dial->blockSignals(true);
   updateDialPosition(dial , spinbox , scale);
   dial->blockSignals(false);
}

double Knob::Value(){
    return spinbox->value();
}


void Knob::setDecimals(int prec){
    spinbox->setDecimals(prec);
    updateWidth(spinbox);
}


void Knob::setKnobColor(QString color){
    dial->setStyleSheet(QString("background-color:") + color);
}

void Knob::setTitle(const QString &title){
    label_title->setText(title);
}

void Knob::setDisabled(bool state){
    dial->setDisabled(state);
    spinbox->setDisabled(state);
    label_title->setDisabled(state);
}

void Knob::setSingleStep(double singleStep){
    spinbox->setSingleStep(singleStep);
}


