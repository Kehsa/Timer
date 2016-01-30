#pragma once

#include <tuple>

#include <QtWidgets/QApplication>
#include <QSqlDatabase>
#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "mytimerkeeper.h"

class MyDataBase :public QObject {
    Q_OBJECT
private:
    QSqlDatabase base;

public:
    MyDataBase(QString name);
    ~MyDataBase();

    std::vector<std::tuple<QString, QString, int, int, int> > getTimers();
    void saveTimers(const std::vector<MyTimerKeeper*>& timerList);
    QSqlDatabase getBase() const;
};
