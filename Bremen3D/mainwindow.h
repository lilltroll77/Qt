#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAGIC_FILECHECKNUMBER 17335633962690440504
#define FILEVERSION 1

#include <QMainWindow>
#include <QAction>
#include <widget.h>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>



struct EQSection_t{
    bool active=false;
    FilterType type;
    double fc=DEFAULT_FC;
    bool link=false;
    double Q=DEFAULT_Q;
    double gain=DEFAULT_GAIN;

};

struct Channel_t{
    double DACgain=0;
    bool invert=false;
    bool mute=false;
    QString *alias;
    EQSection_t eqsection[SECTIONS];
};

struct Settings_t{
    double mixer=0;
    Channel_t channel[CHANNELS];
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
void programChanged(int program);

private slots:
void slot_about();
void slot_saveas();
void slot_open();

private:
int currentProgram;
QMessageBox *msgBoxAbout;
QMessageBox msgBoxError;
QMenuBar *menubar;
QToolBar *fileToolBar;
Widget *central_widget;
QMenu *fileMenu;
QMenu *helpMenu;
QAction *openAction;
QAction *saveAsAction;
QAction *saveAction;
QAction *exitAction;
QAction *aboutAction;
QAction *plotAction;
Settings_t programSettings[4];

};

static MainWindow *MAINWindowRef;

#endif // MAINWINDOW_H
