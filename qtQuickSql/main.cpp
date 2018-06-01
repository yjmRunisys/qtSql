#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <DatabaseHelper.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    CDatabaseHelper mMysql;//数据库
    bool bInit = mMysql.InitDataBase();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
