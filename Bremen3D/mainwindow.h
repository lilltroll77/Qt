#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAGIC_FILECHECKNUMBER 17335633962690440504

#include <QMainWindow>
#include <QAction>
#include <widget.h>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    QWidget *mainwindow;
    ~MainWindow();

signals:

private slots:
void slot_about();
void slot_saveas();
void slot_open();

private:
QMessageBox *msgBoxAbout;
QMessageBox msgBoxError;
QMenuBar *menubar;
QToolBar *fileToolBar;
QWidget *central_widget;
QMenu *fileMenu;
QMenu *helpMenu;
QAction *openAction;
QAction *saveAsAction;
QAction *saveAction;
QAction *exitAction;
QAction *aboutAction;
QAction *plotAction;
};

#endif // MAINWINDOW_H
