#ifndef KNOB_H
#define KNOB_H

#include <QWidget>
#include <QLabel>
#include <QDial>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <math.h>
#include <QString>

enum Scale{linScale,logScale};

class Knob : public QWidget
{
    Q_OBJECT
public:
  explicit Knob(QWidget *parent = 0 , const Scale scaletype = linScale);
  void setRange(double min , double max , int steps);
  double Value();
  void setValue(double value);
  void setKnobColor(QString color);
  void setTitle(const QString &title);
  void setDecimals(int prec);
  void setSingleStep(double singleStep);

signals:
void valueChanged(double Value);

public slots:
void dial_changed(int value);
void spinbox_changed();
void setDisabled(bool state);


private:
bool atMax=false;
bool atMin=false;
Scale scale;
QWidget *widget;
QLabel *label_title;
//QLineEdit *lineEdit_min;
//QLineEdit *lineEdit_max;
QLabel *label_unit;
QDial *dial;
QDoubleSpinBox *spinbox;
QVBoxLayout *layout;

};

#endif // KNOB_H
