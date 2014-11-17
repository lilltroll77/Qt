#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAGIC_FILECHECKNUMBER 17335633962690440504
#define FILEVERSION 5

#include <QMainWindow>
#include <QAction>
#include <widget.h>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include "UDPcommands.h"
#include <QTimer>
#include <QDebug>
#include <QProgressDialog>
#include "windows.h"

struct EQSection_t{
    bool active=false;
    filterType_t type;
    float fc=DEFAULT_FC;
    bool link=false;
    float Q=DEFAULT_Q;
    float gain=DEFAULT_GAIN;
    float MasterGain;
};

struct Channel_t{
    EQSection_t eqsection[SECTIONS];
    double DACgain=0;
    double delay=0;
    bool invert=false;
    bool mute=false;
    QString alias;
};

struct Settings_t{
    double mixer=0;
    double linkedFc;
    Channel_t channel[CHANNELS];
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    bool getMuteState();
    void syncFromHost();
    void waitForPING();
    QStatusBar *statusbar;
    ~MainWindow();

signals:

private slots:
void slot_about();
void slot_saveas();
void slot_open();
void slot_graph_options();
void slot_networkSettings();
void slot_syncToHost(bool button);
void slot_syncFromHost(bool button);
void slot_IP_changed();
void slot_Port_changed();

private:
void pause(int t);
int currentProgram;
QMessageBox *msgBoxAbout;
QMessageBox msgBoxGraph;
QMessageBox msgBoxError;
QMenuBar *menubar;
QToolBar *fileToolBar;
Widget *central_widget;
QMenu *fileMenu;
QMenu *networkMenu;
QMenu *options;
QMenu *helpMenu;
QAction *openAction;
QAction *openActionToolbar;
QAction *saveAsAction;
QAction *saveAsActionToolbar;
QAction *saveAction;
QAction *exitAction;
QAction *networkAction;
QAction *syncToHostAction;
QAction *syncFromHostAction;
QAction *aboutAction;
QAction *plotAction;
QAction *graphAction;
Settings_t programSettings[16];
QTimer tmr;
QProgressDialog* progress;
Network ethernet;
};


#endif // MAINWINDOW_H
