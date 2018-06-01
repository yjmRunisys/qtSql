#include "DatabaseHelper.h"
#include "QDebug"
#include "qThread"

CDatabaseHelper::CDatabaseHelper()
{


}

CDatabaseHelper::~CDatabaseHelper()
{
    StorageDataBase.close();

}

///////////////////////////
bool CDatabaseHelper::InitDataBase()
{
    //设置相关参数
    m_hostName = "192.168.0.162";// 主机名(地址)
    m_databaseName = "dbuavflight";// 需要与数据库中的一致
    m_username = "root";//设置用户名
    m_password = "123456";//设置密码
    m_Port = 3306;// 端口
    ConnectionPool& pool = ConnectionPool::getInstance();
    // 数据库信息传给连接池
    pool.hostName =m_hostName;
    pool.databaseName =m_databaseName;
    pool.username =m_username;
    pool.password =m_password;
    pool.Port =m_Port;

    // 打开数据库
    StorageDataBase = QSqlDatabase::addDatabase("QMYSQL");
    StorageDataBase.setHostName(m_hostName);
    StorageDataBase.setDatabaseName(m_databaseName);
    StorageDataBase.setUserName(m_username);
    StorageDataBase.setPassword(m_password);
    StorageDataBase.setPort(m_Port);

    bool bopen = StorageDataBase.open();
    if(!bopen)
    {
        qDebug()<<"#####open database fail";
        return false;
    }else{
        database_tables = Get_TableNames();
         qDebug()<<"#####open database sucess";
    }

    executeSql("use dbuavflight");

    //QList<QString> mKeys = getDbFields("p_user");

   testMysql();

    return true;

}

//测试数据库
void CDatabaseHelper::testMysql()
{
    //测试写入
//    ConditionType mCondition;
//    QString tableName = "p_user";
//    for(int i= 0;i<100;i++)
//    {
//        QString qsName = QString("bj_%1").arg(i);
//        mCondition["name"] = qsName;
//        mCondition["password"] = "bj";
//        mCondition["user_right"] = "1";
//        Add(tableName,mCondition);
//    }

    //测试查询
//    ConditionType mCondition,mCondition0;
//    ResultType mResult;
//    QString tableName = "p_user";
//    mCondition["name"] = "bj_1";
//    findWhere(tableName,mCondition,mCondition);
//    selectWhere(tableName,mCondition,mResult);
//    selectLimit(tableName,0,10,mCondition0,mResult);

    //测试修改
//    ConditionType paraArray,request;
//    QString tableName = "p_user";
//    paraArray["password"] = "yjm";
//    request["name"] = "bj_1";
//    Save(tableName,paraArray,request);

//    //测试查询,一个时间戳
//    ConditionType mCondition;
//    ResultType mResult;
//    QString tableName = "p_log";
//    mCondition["level"] = "0";
//    QString timeStart = "2018/01/24 10:05:57.223";
//    QString timeEnd = "2018/01/24 10:13:38.346";
//    QString timeField = "time";
//    selectWhere(tableName,mCondition,timeStart,timeEnd,timeField,mResult);


    //测试查询,二个时间戳
    ConditionType mCondition;
    ResultType mResult;
    QString tableName = "t_spectrum";
    mCondition["recordTime"] = "11";
    QString timeStart = "2018/01/30 13:38:18.432";
    QString timeEnd = "2018/01/30 14:43:03.307";
    QString timeField0 = "time_start";
    QString timeField1 = "time_end";
    selectWhere(tableName,mCondition,timeStart,timeEnd,timeField0,timeField1,mResult);


}

//获取数据库最大sid
int CDatabaseHelper::Get_TableMaxSid(QString tableName)
{
    QString sql_use = "SELECT * from " + tableName.toUpper() + " where SID = (SELECT max(SID) FROM "
            + tableName.toUpper() + ")";
    ResultType mResult = querySql(tableName,sql_use);
    if(!mResult.isEmpty())
    {
        return (mResult[0]["sid"]).toInt();
    }else{
        return -1;
    }
}

//返回结果的执行语句
 int CDatabaseHelper::execute(QString sqlcom)
 {
     if(StorageDataBase.isOpen())
     {
         QSqlDatabase db = ConnectionPool::openConnection();
         QSqlQuery query(db);
         bool bFlag = query.exec(sqlcom);
         ConnectionPool::closeConnection(db);        

         return (int)bFlag;
     }else{
         qDebug()<<"database is not open";
     }

     return 0;
 }

 //返回结果的sql语句
 ResultType CDatabaseHelper::querySql(QString tableName,QString sql_str)
 {
     //获取table的key，不对应的key剔除
     QList<QString> mKeys = getDbFields(tableName);

     //执行语句
     QSqlDatabase db = ConnectionPool::openConnection();
     QSqlQuery query(db);
     bool bFlag = query.exec(sql_str);
     ConnectionPool::closeConnection(db);

     ResultType mResult;
     if(bFlag)
     {
         //取出结果
         PutSqlResult(query,mKeys,mResult);

     }

     return mResult;

 }

//执行sql语句
bool CDatabaseHelper::executeSql(QString sqlCom)
{
    if(StorageDataBase.isOpen())
    {
        QSqlDatabase db = ConnectionPool::openConnection();
        QSqlQuery query(db);
        bool bFlag = query.exec(sqlCom);
        ConnectionPool::closeConnection(db);

        return bFlag;
    }else{
        qDebug()<<"database is not open";
    }

    return false;
}

//执行sql语句，返回结果
bool CDatabaseHelper::executeSql(QSqlQuery *query,QString sqlCom)
{
    if(StorageDataBase.isOpen())
    {
        QSqlDatabase db = ConnectionPool::openConnection();
        query = new QSqlQuery(db);
        bool bFlag = query->exec(sqlCom);
        ConnectionPool::closeConnection(db);

        return bFlag;
    }else{
        qDebug()<<"database is not open";
    }

    return false;
}

//获取数据库表名
QList<QString> CDatabaseHelper::Get_TableNames()
{
    QList<QString> mTables;

    QString sql_str = "show tables;";//获取字段名
    //QString sql_str = "select * from p_user";//获取字段名

    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec(sql_str);
    ConnectionPool::closeConnection(db);

    while(query.next())
    {
        QString qstr = query.value(0).toString();
        qDebug() << "table name:" << qstr;
        mTables.push_back(qstr);
    }

    return mTables;
}

//获取数据库表相应key
QList<QString> CDatabaseHelper::getDbFields(QString tableName)
{
    QList<QString> mKeys;
    QString sql_str = "show columns from `" + tableName.toUpper() + "`";
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    query.exec(sql_str);
    ConnectionPool::closeConnection(db);

    while(query.next())
    {
        QString qstr= query.value("Field").toString();
        mKeys.push_back(qstr);
    }

    return mKeys;
}

//取数据结果
void CDatabaseHelper::PutSqlResult(QSqlQuery query,QList<QString> mKeys,ResultType& mResult)
{
    mResult.clear();
    int nKey = mKeys.count();
    while(query.next())
    {
        ConditionType sqlResult;
        for(int i=0;i<nKey;i++)
        {
            QString value = query.value(i).toString();
            sqlResult.insert(mKeys[i],value);
        }
        mResult.push_back(sqlResult);
    }

}

///************************************
/// 数据库条件查询
/// QString tableName 库表名
/// ConditionType paraArray 条件
/// ResultType& mResult 返回结果
///************************************
int CDatabaseHelper::selectWhere(QString tableName,ConditionType paraArray,ResultType& mResult)
{
    //判断是否包含数据表
    if(!database_tables.contains(tableName))
    {
        qDebug()<<"数据库表"<<tableName<<"不存在";
        return 0;
    }

    //获取table的key，不对应的key剔除
    QList<QString> mKeys = getDbFields(tableName);

    //组装sql语句
    QString sql_str = "SELECT * FROM `" +  tableName.toUpper() + "`";
    if(!paraArray.isEmpty())
    {
        int nKey = paraArray.count();
        if(nKey >0)
        {
            sql_str = sql_str + " where ";
            int ii = 0;
            foreach (const QString &key, paraArray.keys())
            {
                QString value = paraArray.value(key);
                if(value == "")
                {
                    sql_str = sql_str + "`" + key + "` is NULL";
                }else{
                    sql_str = sql_str + "`" + key + "` = '" + value + "'";
                }

                ii++;
                if(ii < nKey)
                {
                    sql_str = sql_str + " and ";
                }

            }
        }
    }

    if(mKeys.contains("SID"))
    {
        sql_str = sql_str + " ORDER BY \"SID\"";
    }

    //执行语句
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool bFlag = query.exec(sql_str);
    ConnectionPool::closeConnection(db);

    if(bFlag)
    {
        //取出结果
        PutSqlResult(query,mKeys,mResult);
        return 1;
    }else{
        return 0;
    }


}

///************************************
/// 数据库条件查询
/// QString tableName 库表名
/// int itemFirst 查询的结果第一条
/// int itemSize 查询的结果数目
/// ConditionType paraArray 条件
/// ResultType& mResult 返回结果
///************************************
 int CDatabaseHelper::selectLimit(QString tableName,int itemFirst,int itemSize,ConditionType paraArray,ResultType& mResult)
 {
     //判断是否包含数据表
     if(!database_tables.contains(tableName))
     {
         qDebug()<<"数据库表"<<tableName<<"不存在";
         return 0;
     }

     int itemLast = itemFirst + itemSize + 1;

     //获取table的key，不对应的key剔除
     QList<QString> mKeys = getDbFields(tableName);

     //组装sql语句
     QString sql_str = "SELECT * FROM `" +  tableName.toUpper() + "`";
     int nKey = 0;
     if(!paraArray.isEmpty())
     {
         nKey = paraArray.count();
         if(nKey >0)
         {
             sql_str = sql_str + " where ";
             int ii = 0;
             foreach (const QString &key, paraArray.keys())
             {
                 QString value = paraArray.value(key);
                 if(value == "")
                 {
                     sql_str = sql_str + "`" + key + "` is NULL";
                 }else{
                     sql_str = sql_str + "`" + key + "` = '" + value + "'";
                 }

                 ii++;
                 if(ii < nKey)
                 {
                     sql_str = sql_str + " and ";
                 }

             }
         }
     }

     //前面有无搜索条件
     if(nKey > 0)
     {
        sql_str = sql_str + " LIMIT " + QString("%1,%2").arg(itemFirst).arg(itemSize);
     }else{
        sql_str = sql_str + " LIMIT " + QString("%1,%2").arg(itemFirst).arg(itemSize);
     }

     //有SID则有序排列
     if(mKeys.contains("SID"))
     {
         sql_str = sql_str + " ORDER BY \"SID\"";
     }

     //执行语句
     QSqlDatabase db = ConnectionPool::openConnection();
     QSqlQuery query(db);
     bool bFlag = query.exec(sql_str);
     ConnectionPool::closeConnection(db);

     if(bFlag)
     {
         //取出结果
         PutSqlResult(query,mKeys,mResult);
         return 1;
     }else{
         return 0;
     }
 }

////************************************************
/// QString tableName 库表名
/// int sid 序号
///
///
///
/// ************************************************
int CDatabaseHelper::deleteSid(QString tableName,int sid)
{
    //判断是否包含数据表
    if(!database_tables.contains(tableName))
    {
        qDebug()<<"数据库表"<<tableName<<"不存在";
        return 0;
    }

    //组装sql语句
    QString sql_str = "DELETE * FROM `" +  tableName.toUpper() + "` WHERE `SID` = " + sid;

    return execute(sql_str);
}

///************************************
/// 数据库保存
/// QString tableName 库表名
/// ConditionType paraArray 插入结果
/// ConditionType request 查询条件
///************************************
int CDatabaseHelper::Save(QString tableName,ConditionType paraArray,ConditionType request)
{
    //判断是否包含数据表
    if(!database_tables.contains(tableName))
    {
        qDebug()<<"数据库表"<<tableName<<"不存在";
        return 0;
    }

    //获取table的key，不对应的key剔除
    QList<QString> mKeys = getDbFields(tableName);

    //组装sql语句
    QString sql_str = "UPDATE `" +  tableName.toUpper() + "` SET ";

    //插入数据
    if(!paraArray.isEmpty())
    {
        int nKey = paraArray.count();
        if(nKey >0)
        {
            int ii = 0;
            foreach (const QString &key, paraArray.keys())
            {
                QString value = paraArray.value(key);
                sql_str = sql_str + "`" + key + "` = '" + value + "'";

                ii++;
                if(ii < nKey)
                {
                    sql_str = sql_str + " , ";
                }

            }
        }
    }

    //执行条件
    if(!request.isEmpty())
    {
        int nKey = request.count();
        if(nKey >0)
        {
            sql_str = sql_str + " where ";
            int ii = 0;
            foreach (const QString &key, request.keys())
            {
                QString value = request.value(key);
                if(value == "")
                {
                    sql_str = sql_str + "`" + key + "` is NULL";
                }else{
                    sql_str = sql_str + "`" + key + "` = '" + value + "'";
                }

                ii++;
                if(ii < nKey)
                {
                    sql_str = sql_str + " and ";
                }

            }
        }
    }

    //执行语句
    return execute(sql_str);
}

//找出第一条结果
int CDatabaseHelper::findWhere(QString tableName,ConditionType paraArray,ConditionType& mResult)
{
    ResultType mResultArray;
    selectWhere(tableName,paraArray,mResultArray);
    if(mResultArray.count() > 0)
    {
        mResult = mResultArray[0];
    }

    return 1;
}

///************************************
/// 插入语句
/// QString tableName 库表名
/// ConditionType paraArray 插入结果
/// ConditionType request 查询条件
///************************************
int CDatabaseHelper::Add(QString tableName,ConditionType paraArray)
{
    //判断是否包含数据表
    if(!database_tables.contains(tableName))
    {
        qDebug()<<"数据库表"<<tableName<<"不存在";
        return 0;
    }

    //获取table的key，不对应的key剔除
    //QList<QString> mKeys = getDbFields(tableName);

    //组装sql语句
    QString sql_str = "insert into `" +  tableName.toUpper() + "` (";

    //插入数据
    if(!paraArray.isEmpty())
    {
        int nKey = paraArray.count();
        if(nKey >0)
        {
            //拼写key
            int ii = 0;
            foreach (const QString &key, paraArray.keys())
            {
                sql_str = sql_str + "`" + key + "`";
                ii++;
                if(ii < nKey)
                {
                    sql_str = sql_str + ",";
                }else{
                    sql_str = sql_str + ")";
                }

            }

            //拼写内容
            ii = 0;
            sql_str = sql_str + " values(";
            foreach (const QString &key, paraArray.keys())
            {
                QString value = paraArray.value(key);
                sql_str = sql_str + "'" + value + "'";
                ii++;
                if(ii < nKey)
                {
                    sql_str = sql_str + ",";
                }else{
                    sql_str = sql_str + ")";
                }

            }
        }
    }

    //执行语句
    return execute(sql_str);

}

///************************************
/// 数据库条件查询
/// QString tableName 库表名
/// ConditionType paraArray 条件
/// ResultType& mResult 返回结果
/// QString timeStart 开始时间
/// QString timeEnd 结束时间
/// QString timeFieldName 时间戳字段名
///************************************
int CDatabaseHelper::selectWhere(QString tableName,ConditionType paraArray,
                QString timeStart,QString timeEnd,QString timeField,ResultType& mResult)
{

    //判断是否包含数据表
    if(!database_tables.contains(tableName))
    {
        qDebug()<<"数据库表"<<tableName<<"不存在";
        return 0;
    }

    //获取table的key，不对应的key剔除
    QList<QString> mKeys = getDbFields(tableName);

    //组装sql语句
    QString sql_str = "SELECT * FROM `" +  tableName.toUpper() + "`";
    sql_str = sql_str + " where (`" + timeField.toUpper() + "` between '"+timeStart+ "' and '" + timeEnd + "')";
    if(!paraArray.isEmpty())
    {
        int nKey = paraArray.count();
        if(nKey >0)
        {
            sql_str = sql_str + " and ";
            int ii = 0;
            foreach (const QString &key, paraArray.keys())
            {
                QString value = paraArray.value(key);
                if(value == "")
                {
                    sql_str = sql_str + "`" + key + "` is NULL";
                }else{
                    sql_str = sql_str + "`" + key + "` = '" + value + "'";
                }

                ii++;
                if(ii < nKey)
                {
                    sql_str = sql_str + " and ";
                }

            }
        }
    }

    if(mKeys.contains("SID"))
    {
        sql_str = sql_str + " ORDER BY \"SID\"";
    }

    //执行语句
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool bFlag = query.exec(sql_str);
    ConnectionPool::closeConnection(db);

    if(bFlag)
    {
        //取出结果
        PutSqlResult(query,mKeys,mResult);
        return 1;
    }else{
        return 0;
    }

}

///************************************
/// 数据库条件查询
/// QString tableName 库表名
/// ConditionType paraArray 条件
/// ResultType& mResult 返回结果
/// QString timeStart 开始时间
/// QString timeEnd 结束时间
/// QString timeField0 开始时间戳字段名
/// QString timeField0 结束时间戳字段名
///************************************
int CDatabaseHelper::selectWhere(QString tableName,ConditionType paraArray,
                QString timeStart,QString timeEnd,
                QString timeField0,QString timeField1,
                ResultType& mResult)
{
    //判断是否包含数据表
    if(!database_tables.contains(tableName))
    {
        qDebug()<<"数据库表"<<tableName<<"不存在";
        return 0;
    }

    //获取table的key，不对应的key剔除
    QList<QString> mKeys = getDbFields(tableName);

    //组装sql语句
    QString sql_str = "SELECT * FROM `" +  tableName.toUpper() + "`";
    sql_str = sql_str + " where ";
    QString requre1 = "(`" + timeField0.toUpper() + "` between '"
            +timeStart+ "' and '" + timeEnd + "')";
    QString requre2 = "(`" + timeField1.toUpper() + "` between '"
            +timeStart+ "' and '" + timeEnd + "')";
    QString requre3 = "( (`" + timeField0.toUpper() + "` <= '"+timeStart + "')"
            + " and (`" + timeField1.toUpper() + "` >= '"+timeEnd + "') )";
    sql_str = sql_str + "(" + requre1 + " or " + requre2 + " or " + requre3 + ")";
    if(!paraArray.isEmpty())
    {
        int nKey = paraArray.count();
        if(nKey >0)
        {
            sql_str = sql_str + " and ";
            int ii = 0;
            foreach (const QString &key, paraArray.keys())
            {
                QString value = paraArray.value(key);
                if(value == "")
                {
                    sql_str = sql_str + "`" + key + "` is NULL";
                }else{
                    sql_str = sql_str + "`" + key + "` = '" + value + "'";
                }

                ii++;
                if(ii < nKey)
                {
                    sql_str = sql_str + " and ";
                }

            }
        }
    }

    if(mKeys.contains("SID"))
    {
        sql_str = sql_str + " ORDER BY \"SID\"";
    }

    //执行语句
    QSqlDatabase db = ConnectionPool::openConnection();
    QSqlQuery query(db);
    bool bFlag = query.exec(sql_str);
    ConnectionPool::closeConnection(db);

    if(bFlag)
    {
        //取出结果
        PutSqlResult(query,mKeys,mResult);
        return 1;
    }else{
        return 0;
    }
}

//////////////////////////////

bool CDatabaseHelper::createTable()
{
    if (StorageDataBase.isOpen())
    {
        QSqlDatabase db = ConnectionPool::openConnection();
        QSqlQuery query(db);
        QString createDB = "create table DataExportConf( id int primary key, tar bool ,"
                           "compressConf int, compressLevel int, diskClone bool,dataSrc text,excludeDir text ,"
                           "dataDst text , isFinished bool, isValid bool)";
        if(query.exec(createDB))
            qDebug() << "DataExportConf table create successful";
        else
            qDebug() << "DataExportConf table create fail";

        createDB = "create table DataExportList(id int primary key,dataExportConfNum int ,dataPath text,isFinished bool)";
        if(query.exec(createDB))
            qDebug() << "DataExportList table create successful";
        else
            qDebug() << "DataExportList table create fail";
        createDB = "create table RemoteForderParam ( id int primary key,remoteAddr text,port int ,"
                   "filDir text,user text,password text,startTime text,cycle int ,lastResult text,isValid bool )";
        if(query.exec(createDB))
            qDebug() << "RemoteForderParm table create successful";
        else
            qDebug() << "remoteForderParm table create fail";

        createDB = "create table TaskChanged ( id int primary key,param bool ,dataExport bool ,remoteFileBackup bool ,remoteDBmanualBackup bool )";
        if(query.exec(createDB))
            qDebug() << "TaskChanged table create successful";
        else
            qDebug() << "TaskChanged table create fail";


        bool ret = query.exec("create table LogInfo"

                              "(id integer primary key, "
                              "Time time, "
                              "Status text, "
                              "content text)");
        ConnectionPool::closeConnection(db);

        if (ret) {
            qDebug() << "table created ";
            return true;
        } else {
            qDebug() << "faied to crate table";
            return false;
        }
    } else {
        return false;
    }
}

bool CDatabaseHelper::dropTable()
{

    if (StorageDataBase.isOpen()) {
        QSqlDatabase db = ConnectionPool::openConnection();
        QSqlQuery query(db);
        //QSqlQuery query(StorageDataBase);
        bool ret = query.exec("drop table studentTable");
        ConnectionPool::closeConnection(db);
        if (ret)
        {
            qDebug() << "table dropped";
            return true;
        } else {
            qDebug() << "faied to drop the table";
            return false;
        }
    } else {
        return false; //return false if failed to open database
    }
}

bool CDatabaseHelper::insertAction(QJsonDocument jsondoc){
    //    bool flag = false;
    //    QJsonParseError error;
    QVariantMap result;
    QVariantMap nestedMap;
    QVariant tableNmae;

    if (jsondoc.isObject()) {
        result = jsondoc.toVariant().toMap();
        //qDebug() << "encoding:" << result["encoding"].toString();
        //qDebug() << "plugins:";

        //foreach (QVariant plugin, result["plug-ins"].toList()) {
        //    qDebug() << "\t-" << plugin.toString();
        //}
        if(result.contains("data")&&result.contains("tableName")){
            nestedMap = result["data"].toMap();
            tableNmae = result["tableName"];

        }
        else{
            qDebug() << "MISSING THE 'data' OR 'tableName' KEY!!";
            return false;

        }


        // qDebug() << "length:" << nestedMap["length"].toInt();
        //qDebug() << "use_space:" << nestedMap["use_space"].toBool();
    }

    if (StorageDataBase.isOpen()) {
        QSqlDatabase db = ConnectionPool::openConnection();
        QSqlQuery query(db);
        //QSqlQuery query(StorageDataBase);
        QString sql ="INSERT INTO " + tableNmae.toString() + " (";
        QString value = "VALUES (";
        QMapIterator<QString,QVariant> i(nestedMap);
        while(i.hasNext()){
            i.next();
            sql += i.key();
            value += "?";
            if (i.hasNext()){
                sql += ",";
                value += ",";
            }
        }
        sql += ") "+value+")";
        query.prepare(sql);
        i.toFront();
        while(i.hasNext()){
            i.next();
            //            qDebug() << i.key();
            //            qDebug() << i.value();
            //            qDebug() << nestedMap.value(i.key());

            query.addBindValue(i.value());

        }
        bool ret = query.exec();
        ConnectionPool::closeConnection(db);
        if (ret) {
            //            qDebug() << "INSERT DONE!!";
            //             qDebug() << "sql:"<<sql;
            return true;
        } else {
            qDebug() << "sql:"<<sql;
            qDebug() << "INSERT FAILE!!";
            return false;
        }
    } else {
        qDebug() << "FAILE OPEN MYSQL!!";
        return false; //return false if failed to open database
    }

}

bool CDatabaseHelper::updateAction(QJsonDocument jsondoc){
    QVariantMap result;
    QVariantMap setMap;
    QVariantMap filterMap;
    QVariant tableNmae;

    if (jsondoc.isObject()) {
        result = jsondoc.toVariant().toMap();
        if(result.contains("set")&&result.contains("tableName")&&result.contains("filter")){
            setMap = result["set"].toMap();
            filterMap = result["filter"].toMap();
            tableNmae = result["tableName"];
        }
        else{
            qDebug() << "MISSING THE 'set' OR 'tableName' OR 'filter' KEY!!";
            return false;

        }
    }

    if (StorageDataBase.isOpen())
    {
        QSqlDatabase db = ConnectionPool::openConnection();
        QSqlQuery query(db);
        //QSqlQuery query(StorageDataBase);
        QString sql ="UPDATE " + tableNmae.toString() + " SET ";
        QString filter = " WHERE ";
        QMapIterator<QString,QVariant> i(setMap);
        while(i.hasNext())
        {
            i.next();
            //            QString val ='\"' + i.value().toString() +'\"';

            sql += i.key() + "=" + "'" + i.value().toString() + "'";
            if (i.hasNext()){
                sql += ",";
            }
        }

        QMapIterator<QString,QVariant> j(filterMap);
        while(j.hasNext()){
            j.next();
            filter += j.key() + "=" + "'" + j.value().toString() + "'";;
            if(j.hasNext())
                filter += " and ";
        }
        sql += filter;


        query.prepare(sql);
        bool ret = query.exec();
        ConnectionPool::closeConnection(db);
        if (ret) {
                        qDebug() << "UPDATE DONE!!";
                       qDebug() << "sql:"<<sql;
            return true;
        } else {
            qDebug() << "sql:"<<sql;
            qDebug() << "UPDATE FAILE!!";
            return false;
        }
    } else {
        qDebug() << "FAILE OPEN MYSQL!!";
        return false; //return false if failed to open database
    }

}

bool CDatabaseHelper::InsertLog(const char *content, short level, char *source)
{

    //SysMonitor sysMonitor;
    QJsonObject json,jsonData;
    QJsonDocument jsondoc;
    //QString cInsertlogTime;
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString cInsertlogTime = time.toString("yyyy-MM-dd hh:mm:ss "); //设置显示格式

    jsonData.insert("time",cInsertlogTime);
    jsonData.insert("source",source);
    jsonData.insert("content",content);
    jsonData.insert("level",level);
    json.insert("tableName","DevLog");
    json.insert("data",jsonData);
    jsondoc.setObject(json);
    bool bsert = insertAction(jsondoc);
    return bsert;
}
