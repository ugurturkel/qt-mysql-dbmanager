#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>

class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);
    ~DBManager();

    void initDbModel(QString);
    bool connectDB(QString, quint16, QString, QString, QString);
    void closeDB();
    QSqlRecord select(QStringList, QVector<QVariant>, QString);
    void insertRow(QStringList, QVector<QVariant>);

private:
    QSqlDatabase db;
    QSqlTableModel* dbModel;
signals:

};

#endif // DBMANAGER_H
