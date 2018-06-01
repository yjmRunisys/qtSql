#include "brower.h"
//#include "ui_brower.h"

#include <QtSql>
#include <QtWidgets>

Browser::Browser(QWidget *parent) :
    QWidget(parent)
    //,ui(new Ui::Brower)
{
    //ui->setupUi(this);
    setupUi(this);

//    QGroupBox* gBox = new QGroupBox;
//    //setCentralWidget(gBox);
//    QGridLayout* layout = new QGridLayout(this);
//    layout->addWidget(connectionWidget,0,0,1,1);
//    layout->addWidget(table , 0 , 2 , 1 , 2);
//    gBox->setLayout(layout);


    InitAction();

    //右键出菜单
    table->setContextMenuPolicy(Qt::ActionsContextMenu);

    if (QSqlDatabase::drivers().isEmpty())
    {
        QMessageBox::information(this, tr("No database drivers found"),
                                 tr("This demo requires at least one Qt database driver. "
                                    "Please check the documentation how to build the "
                                    "Qt SQL plugins."));
    }

    InitDb();

}

Browser::~Browser()
{
    //delete ui;
}

void Browser::InitAction()
{
    insertRowAction = new QAction(tr("insertRowAction"),this);
    insertRowAction->setEnabled(false);
    insertRowAction->setText(QApplication::translate("Browser", "&Insert Row", 0));
    table->addAction(insertRowAction);

    deleteRowAction = new QAction(this);
    deleteRowAction->setEnabled(false);
    deleteRowAction->setText(QApplication::translate("Browser", "&Delete Row", 0));
    table->addAction(deleteRowAction);

    fieldStrategyAction = new QAction(this);
    fieldStrategyAction->setCheckable(true);
    fieldStrategyAction->setText(QApplication::translate("Browser", "Submit on &Field Change", 0));
    table->addAction(fieldStrategyAction);

    rowStrategyAction = new QAction(this);
    rowStrategyAction->setCheckable(true);
    rowStrategyAction->setText(QApplication::translate("Browser", "Submit on &Row Change", 0));
    table->addAction(rowStrategyAction);

    manualStrategyAction = new QAction(this);
    manualStrategyAction->setCheckable(true);
    manualStrategyAction->setText(QApplication::translate("Browser", "Submit &Manually", 0));
    table->addAction(manualStrategyAction);

    submitAction = new QAction(this);
    submitAction->setText(QApplication::translate("Browser", "&Submit All", 0));
    table->addAction(submitAction);

    revertAction = new QAction(this);
    revertAction->setText(QApplication::translate("Browser", "&Revert All", 0));
    table->addAction(revertAction);

    selectAction = new QAction(this);
    selectAction->setText(QApplication::translate("Browser", "S&elect", 0));
    table->addAction(selectAction);

}

bool Browser::InitDb()
{
    QSqlError err = addConnection(
                CFG_SQL_DRIVER, CFG_SQL_DBNAME, CFG_SQL_HOST,
                CFG_SQL_USER, CFG_SQL_PASSWORD, CFG_SQL_PORT);
    if (err.type() == QSqlError::NoError)
    {
//        QMessageBox::information(this, tr("Open database success"), tr("Open datases %1 success")
//                                 .arg(CFG_SQL_DBNAME));
        return false;
    }else{
        QMessageBox::warning(this, tr("Unable to open database"), tr("An error occurred while "
                                   "opening the connection: ") + err.text());
        return true;
    }
}

QSqlError Browser::addConnection(const QString &driver, const QString &dbName, const QString &host,
              const QString &user, const QString &passwd, int port)
{
    static int cCount = 0;

    QSqlError err;
    QSqlDatabase db = QSqlDatabase::addDatabase(driver, QString("Browser%1").arg(++cCount));
    db.setDatabaseName(dbName);
    db.setHostName(host);
    db.setPort(port);
    if (!db.open(user, passwd)) {
        err = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(QString("Browser%1").arg(cCount));
    }
    connectionWidget->refresh();

    return err;
}

void Browser::showTable(const QString &t)
{
    QSqlTableModel *model = new /*CustomModel*/QSqlTableModel(table, connectionWidget->currentDatabase());
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setTable(connectionWidget->currentDatabase().driver()->
                    escapeIdentifier(t, QSqlDriver::TableName));
    model->select();
    if (model->lastError().type() != QSqlError::NoError)
        emit statusMessage(model->lastError().text());
    table->setModel(model);

    //双击或者按下编辑按钮则可编辑
    table->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);

    //行改变则触发currentChanged
    connect(table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentChanged()));

    updateActions();
}

//刷新
void Browser::updateActions()
{
    QSqlTableModel * tm = qobject_cast<QSqlTableModel *>(table->model());
    bool enableIns = tm;
    bool enableDel = enableIns && table->currentIndex().isValid();

    insertRowAction->setEnabled(enableIns);
    deleteRowAction->setEnabled(enableDel);

    fieldStrategyAction->setEnabled(tm);
    rowStrategyAction->setEnabled(tm);
    manualStrategyAction->setEnabled(tm);
    submitAction->setEnabled(tm);
    revertAction->setEnabled(tm);
    selectAction->setEnabled(tm);

    if (tm) {
        QSqlTableModel::EditStrategy es = tm->editStrategy();

        //根据当前策略设置Action是否checked
        fieldStrategyAction->setChecked(es == QSqlTableModel::OnFieldChange);
        rowStrategyAction->setChecked(es == QSqlTableModel::OnRowChange);
        manualStrategyAction->setChecked(es == QSqlTableModel::OnManualSubmit);
    }
}

