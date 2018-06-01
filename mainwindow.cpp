#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "brower.h"
#include "QtWidgets"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QRect mrect = geometry();
    setWindowTitle(QObject::tr("数据库测试"));

    browser = new Browser(this);
    // browser->setGeometry(mrect);
    browser->setWindowTitle(QObject::tr("Brower"));
    //setCentralWidget(browser);

    QObject::connect(browser, SIGNAL(statusMessage(QString)),
                         statusBar(), SLOT(showMessage(QString)));

    QGroupBox* gBox = new QGroupBox;
    setCentralWidget(gBox);
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(browser , 0 , 0 , 1 , 1);
    gBox->setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete ui;
}
