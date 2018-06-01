#ifndef CDATABASEHELPER_H
#define CDATABASEHELPER_H
#include <connectionpool.h>
#include <QJsonDocument>
#include <QMap>

typedef QMap<QString,QString> ConditionType;//条件类型
typedef QList<QMap<QString,QString>> ResultType;//数据库执行结果

class CDatabaseHelper : public QSqlDatabase
{
public:
    CDatabaseHelper();
    ~CDatabaseHelper();
    QSqlDatabase StorageDataBase;
public:
    //yjm代码
    bool InitDataBase();

    //QSqlDatabase db;//执行语句时使用，多线程时使用，打开的连接名和关闭的连接名可能不一致，
    //QSqlQuery *query;//执行语句时使用, 接收结果，使用完清零
    int execute(QString sqlcom);//返回结果的执行语句
    ResultType querySql(QString tableName,QString sqlcom);//返回结果的sql语句

    bool executeSql(QString sqlCom);//执行sql语句
    bool executeSql(QSqlQuery *query,QString sqlCom);//执行sql语句，返回结果
    QList<QString> Get_TableNames();//获取数据库表名
    QList<QString> database_tables;//数据库表名
    QList<QString> getDbFields(QString tableName);//获取数据库表相应key
    void PutSqlResult(QSqlQuery query,QList<QString> mKeys,ResultType& mResult);//取数据结果

    int selectWhere(QString tableName,ConditionType paraArray,ResultType& mResult);    
    int selectLimit(QString tableName,int itemFirst,int itemSize,ConditionType paraArray,ResultType& mResult);
    int deleteSid(QString tableName,int sid);
    int Save(QString tableName,ConditionType paraArray,ConditionType request);
    int findWhere(QString tableName,ConditionType paraArray,ConditionType& mResult);//找出第一条结果
    int Add(QString tableName,ConditionType paraArray);//插入语句

    //时间段内查询
    //一个时间戳,timeFieldName时间戳字段名
    int selectWhere(QString tableName,ConditionType paraArray,
                    QString timeStart,QString timeEnd,QString timeField,ResultType& mResult);

    //二个时间戳,timeField0和timeField1 起始和结束 时间戳字段名
    int selectWhere(QString tableName,ConditionType paraArray,
                    QString timeStart,QString timeEnd,
                    QString timeField0,QString timeField1,
                    ResultType& mResult);

    void testMysql();//测试数据库

public:
    int Get_TableMaxSid(QString tableName);//获取数据库最大sid
private:
    QString m_hostName;
    QString m_databaseName;
    QString m_username;
    QString m_password;
    QString m_databaseType;
    int m_Port;

public:
    //wwy代码
    bool createTable();
    bool dropTable();
    bool insertAction(QJsonDocument jsondoc);
    bool selectAction(QString sql);
    bool updateAction(QJsonDocument jsondoc);
    bool InsertLog(const char* content, short level, char* source);


};

#endif // CDATABASEHELPER_H
