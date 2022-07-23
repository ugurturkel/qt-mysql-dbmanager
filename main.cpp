#include <QCoreApplication>
#include "dbmanager.h"
#include <QSettings>
#include <QDebug>
#include <QSqlRecord>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSettings settings("testSql", "testSql");
    DBManager dbmanager;
    QStringList insertColumns;
    QStringList selectColumns;
    QVector<QVariant> insertValues;
    QVector<QVariant> selectValues;

    QSqlRecord seleectResponse;

    insertColumns.append("testColumn1");
    insertColumns.append("testColumn2");

    insertValues.append("testval1");
    insertValues.append("testval2");

    selectColumns.append("testColumn1");
    selectColumns.append("testColumn2");
    selectColumns.append("testInt");

    selectValues.append("\'testval1\'");
    selectValues.append("\'testval2\'");
    selectValues.append(5);


    dbmanager.connectDB(settings.value("host").toString(),
                        settings.value("port").toInt(),
                        settings.value("dbName").toString(),
                        settings.value("userName").toString(),
                        settings.value("password").toString());

    dbmanager.initDbModel("table1");

    seleectResponse = dbmanager.select(selectColumns, selectValues, "AND");
    qInfo() << "Select query response: " << seleectResponse.value("testInt");
    dbmanager.insertRow(insertColumns, insertValues);
    dbmanager.closeDB();

    return a.exec();
}
