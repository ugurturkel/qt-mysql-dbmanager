#include "dbmanager.h"

#include <QDebug>
#include <QtSql>
#include <QSqlRecord>


DBManager::DBManager(QObject *parent)
    : QObject{parent}
{

}

DBManager::~DBManager()
{

}

void DBManager::initDbModel(QString tableName)
{
    this->dbModel = new QSqlTableModel(this, this->db);
    this->dbModel->setTable(tableName);
    this->dbModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

bool DBManager::connectDB(QString host, quint16 port, QString dbName, QString userName, QString password)
{
    this->db = QSqlDatabase::addDatabase("QMYSQL");
    this->db.setHostName(host);
    this->db.setPort(port);
    this->db.setDatabaseName(dbName);
    this->db.setUserName(userName);
    this->db.setPassword(password);

    if(!db.open()){
        qWarning() << "Failed to connect to MySql database.";
        qWarning() << db.lastError().text();
        return false;
    }

    qInfo() << "Connected to MySql database succesfully.";
    return true;
}

void DBManager::closeDB()
{
    this->db.close();
    delete this->dbModel;
}

QSqlRecord DBManager::select(QStringList columns, QVector<QVariant> values, QString condition)
{
    QSqlRecord response;
    QString filter;
    for(int i = 0; i < columns.size(); i++){
        if(i < columns.size() - 1){
            filter += columns.at(i) + "=" + values.at(i).toString() + " " + condition + " ";
        }
        else{
            filter += columns.at(i) + "=" + values.at(i).toString();
        }
    }

    this->dbModel->setFilter(filter);
    if(!this->dbModel->select()){
        qWarning() << db.lastError().text();
    }
    response = this->dbModel->record(0);
    qInfo() << "Executed select query: " << this->dbModel->query().lastQuery();
    return response;
}

void DBManager::insertRow(QStringList columns, QVector<QVariant> values)
{
    QSqlRecord record = this->dbModel->record();
    /* since the id field has the autoincrement attribute,
      * it is not necessary to indicate its value,
      * that is why this field of the request is removed.
      */
    record.remove(record.indexOf("id"));


    for(int i = 0; i < columns.size(); i++){
        record.setValue(columns.at(i), values.at(i));
    }
    /*-1 is set to indicate that it will be added to the last row*/
    if(dbModel->insertRecord(-1, record)){
        qInfo() << "Successful insertion.";
        dbModel->submitAll();
    }
    else{
        qWarning() << db.lastError().text();
        db.rollback();
    }
}


