#ifndef ADC_H
#define ADC_H

#include <QWidget>
#include "UDPcommands.h"

class ADC : public QWidget
{
    Q_OBJECT
public:
    explicit ADC(QWidget *parent = 0 ,  Network *udp =0);

signals:

public slots:

};

#endif // ADC_H
