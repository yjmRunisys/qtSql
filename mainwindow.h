#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "brower.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public:
    Browser  *browser;
};

#endif // MAINWINDOW_H
