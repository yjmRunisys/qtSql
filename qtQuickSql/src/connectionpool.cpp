#include "connectionpool.h"
#include <QDebug>

QMutex ConnectionPool::mutex;
QWaitCondition ConnectionPool::waitConnection;
ConnectionPool* ConnectionPool::instance = NULL;

ConnectionPool::ConnectionPool() {
    // 创建数据库连接的这些信息在实际开发的时都需要通过读取配置文件得到，
    // 这里为了演示方便所以写死在了代码里。
////    hostName     = "192.168.0.250";
//     hostName     = "192.168.0.242";  //127.0.0.1
//    databaseName = "storage";
//    username     = "root";
//    password     = "runisys";
//    databaseType = "QMYSQL";
//    Port = 3506;
////    Port = 3306;
    testOnBorrow = true;
    testOnBorrowSql = "SELECT 1";

    maxWaitTime  = 1000;
    waitInterval = 200;
    maxConnectionCount  = 20;
}

ConnectionPool::~ConnectionPool() {
    // 销毁连接池的时候删除所有的连接
    foreach(QString connectionName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach(QString connectionName, unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

ConnectionPool& ConnectionPool::getInstance() {
    if (NULL == instance) {
        QMutexLocker locker(&mutex);

        if (NULL == instance) {
            instance = new ConnectionPool();
        }
    }

    return *instance;
}

void ConnectionPool::release() {
    QMutexLocker locker(&mutex);
    delete instance;
    instance = NULL;
}

int iiOpen=0;//
int iiClose = 0;//
QSqlDatabase ConnectionPool::openConnection()
{
    QString qstr = QString("%1").arg(iiOpen++);

    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName;

    QMutexLocker locker(&mutex);

    //qDebug()<<"openConnection In "<<qstr;

    // 已创建连接数
    int connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();

    // 如果连接已经用完，等待 waitInterval 毫秒看看是否有可用连接，最长等待 maxWaitTime 毫秒
    for (int i = 0;
         i < pool.maxWaitTime
         && pool.unusedConnectionNames.size() == 0 && connectionCount == pool.maxConnectionCount;
         i += pool.waitInterval) {
        waitConnection.wait(&mutex, pool.waitInterval);

        // 重新计算已创建连接数
        connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    }

    if (pool.unusedConnectionNames.size() > 0) {
        // 有已经回收的连接，复用它们
        connectionName = pool.unusedConnectionNames.dequeue();
        //qDebug()<<"openConnection dequeue "<<qstr;
    } else if (connectionCount < pool.maxConnectionCount) {
        // 没有已经回收的连接，但是没有达到最大连接数，则创建新的连接
        connectionName = QString("Connection-%1").arg(connectionCount + 1);
    } else {
        // 已经达到最大连接数
        qDebug() << "Cannot create more connections.";
        return QSqlDatabase();
    }


    //qDebug()<<"get connection:"<<connectionName<<"--"<<qstr;

    // 创建连接
    QSqlDatabase db = pool.createConnection(connectionName);

    // 有效的连接才放入 usedConnectionNames
    if (db.isOpen()) {
        pool.usedConnectionNames.enqueue(connectionName);
        //qDebug()<<"connection:"<<connectionName<<"isOpen";
    }else{
        //qDebug()<<"connection:"<<connectionName<<"Open Failed";
    }

    //qDebug()<<"openConnection out "<<qstr;

    return db;
}

void ConnectionPool::closeConnection(QSqlDatabase connection) {
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName = connection.connectionName();

    QString qstr = QString("%1").arg(iiClose++);
    // 如果是我们创建的连接，从 used 里删除，放入 unused 里
    if (pool.usedConnectionNames.contains(connectionName)) {
        QMutexLocker locker(&mutex);
        pool.usedConnectionNames.removeOne(connectionName);
        pool.unusedConnectionNames.enqueue(connectionName);
        waitConnection.wakeOne();
        //qDebug()<<"release Connection:"<<connectionName<<"--"<<qstr;
    }
}

QSqlDatabase ConnectionPool::createConnection(const QString &connectionName) {
    // 连接已经创建过了，复用它，而不是重新创建
    if (QSqlDatabase::contains(connectionName))
    {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);

        if (testOnBorrow) {
            // 返回连接前访问数据库，如果连接断开，重新建立连接
//            qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
            QSqlQuery query(testOnBorrowSql, db1);

            if (query.lastError().type() != QSqlError::NoError && !db1.open()) {
                qDebug() << "Open datatabase error:" << db1.lastError().text();
                return QSqlDatabase();
            }
        }

        return db1;
    }

    // 创建一个新的连接
    databaseType = "QMYSQL";
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connectionName);
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(username);
    db.setPassword(password);
    db.setPort(Port);
    //db.setConnectOptions("MYSQL_OPT_RECONNECT");
    if (!db.open()) {
        qDebug() << "Open datatabase error:" << db.lastError().text();
        db.setConnectOptions();  //clears the connect option string
		return QSqlDatabase();
    }

    return db;
}
