#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#define MAGIC_FILECHECKNUMBER 7119355304613483

#include <QMainWindow>
#include <QVector>
#include "math.h"
#include "classes/qcustomplot.h"
#include "classes/transform.h"

class GraphWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit GraphWindow(QWidget *parent = 0);
    QCustomPlot *plot;

signals:

public slots:
void updateStatusBar(QMouseEvent* mouse);

private:
    QString name;
    QStatusBar *statusbar;
    const QRect windowGeometry;

};

#endif // GRAPHWINDOW_H
