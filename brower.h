#ifndef BROWER_H
#define BROWER_H

#include <QWidget>
#include "config.h"
#include "ui_brower.h"
#include "config.h"
#include "QMessageBox"
#include <QSqlTableModel>

QT_FORWARD_DECLARE_CLASS(QSqlError)

namespace Ui {
class Brower;
}

//继承自ui::brower，继承方式获取ui控件
class Browser : public QWidget, private Ui::Brower
{
    Q_OBJECT

public:
    explicit Browser(QWidget *parent = 0);
    ~Browser();

private:
    //Ui::Brower *ui;

    bool InitDb();
    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,
                  const QString &user, const QString &passwd, int port = -1);

    void updateActions();

public slots:
    void showTable(const QString &table);//显示数据库表
    void currentChanged()
    {
        updateActions();
    }

    void on_connectionWidget_tableActivated(const QString &table)//激活某张表
    {
//        QMessageBox messageBox;
//        messageBox.setText(table);
//        messageBox.exec();
        showTable(table);
    }

    //显示列表filed数据
    void on_connectionWidget_metaDataRequested(const QString &table)
    {
        QMessageBox messageBox;
        messageBox.setText(table);
        messageBox.exec();
        //showMetaData(table);
    }

signals:
    void statusMessage(const QString &message);

private slots:


private:
    QAction *insertRowAction;
    QAction *deleteRowAction;
    QAction *fieldStrategyAction;
    QAction *rowStrategyAction;
    QAction *manualStrategyAction;
    QAction *submitAction;
    QAction *revertAction;
    QAction *selectAction;

    void InitAction();
};

#endif // BROWER_H
