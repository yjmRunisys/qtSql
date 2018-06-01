#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

//单例模式，使用getInstance获取的都为同一个连接

#include <QtSql>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QMutexLocker>

class ConnectionPool {
public:
    static void release(); // 关闭所有的数据库连接

    //openConnection和closeConnection的操作对象必须一致，注意在多线程中的使用；若使用非局部变量
    //可能导致打开和关闭的连接对象不一致
    static QSqlDatabase openConnection();                 // 获取数据库连接
    static void closeConnection(QSqlDatabase connection); // 释放数据库连接回连接池

    ~ConnectionPool();

    // 数据库信息
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;
    int Port;
    static ConnectionPool& getInstance();

private:
//  static ConnectionPool& getInstance();

    ConnectionPool();
    ConnectionPool(const ConnectionPool &other);
    ConnectionPool& operator=(const ConnectionPool &other);
    QSqlDatabase createConnection(const QString &connectionName); // 创建数据库连接

    QQueue<QString> usedConnectionNames;   // 已使用的数据库连接名
    QQueue<QString> unusedConnectionNames; // 未使用的数据库连接名

//    // 数据库信息
//    QString hostName;
//    QString databaseName;
//    QString username;
//    QString password;
//    QString databaseType;
//    int Port;

    bool    testOnBorrow;    // 取得连接的时候验证连接是否有效
    QString testOnBorrowSql; // 测试访问数据库的 SQL

    int maxWaitTime;  // 获取连接最大等待时间
    int waitInterval; // 尝试获取连接时等待间隔时间
    int maxConnectionCount; // 最大连接数

    static QMutex mutex;
    static QWaitCondition waitConnection;
    static ConnectionPool *instance;
};

#endif // CONNECTIONPOOL_H
