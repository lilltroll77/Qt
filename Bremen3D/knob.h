#ifndef KNOB_H
#define KNOB_H

#include <QWidget>
#include <QLabel>
#include <QDial>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QFrame>


class Knob : public QWidget
{
    Q_OBJECT
public:
  explicit Knob(QWidget *parent = 0);
  void setMinimum(double);
  void setMaximum(double);

signals:

public slots:

private:
QWidget *widget;
QLabel *label_title;
QLabel *label_min;
QLabel *label_max;
QDial *dial;
QDoubleSpinBox *spinbox;
QVBoxLayout *layout;
};



#endif // KNOB_H
