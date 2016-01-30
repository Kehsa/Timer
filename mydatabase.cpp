#include "mydatabase.h"

MyDataBase::MyDataBase(QString name) {
    base = QSqlDatabase::addDatabase("QSQLITE");
    base.setDatabaseName(name);
    base.open();
    QSqlQuery q(base);
    q.exec("PRAGMA AUTO_VACUUM = INCREMENTAL;");
    QString str =
            "CREATE TABLE IF NOT EXISTS timers ("
            "'name' TEXT NOT NULL DEFAULT ('Some Timer'),"
            "'hour' INTEGER,"
            "'min' INTEGER,"
            "'mode' INTEGER NOT NULL DEFAULT (0),"
            "'command' TEXT DEFAULT ('')"
            ")";
    q.exec(str);
}

MyDataBase::~MyDataBase() {
    base.close();
}

std::vector<std::tuple<QString, QString, int, int, int> > MyDataBase::getTimers() {
    QSqlQuery q(base);
    q.exec("SELECT * FROM timers");
    QSqlRecord rec = q.record();
    std::vector<std::tuple<QString, QString, int, int, int> > ls;
    while ( q.next()) {
        ls.push_back(std::make_tuple(q.value(rec.indexOf("name")).toString(),
                                     q.value(rec.indexOf("command")).toString(),
                                     q.value("hour").toInt(), q.value("min").toInt(), q.value("mode").toInt()));
    }
    return ls;
}

void MyDataBase::saveTimers(const std::vector<MyTimerKeeper *> &timerList) {
    QSqlQuery q(base);
    q.exec("DELETE FROM timers");
    for (auto &x : timerList) {
        QString str =
                "INSERT INTO"
                    "`timers` ('name', 'hour', 'min', 'mode', 'command')"
                "VALUES"
                    "('" + x->getName() + "', '" + QString::number(x->getConfTimeH()) + "', '"
                + QString::number(x->getConfTimeM()) + "', '" + QString::number(x->getMode())
                + "', '" + x->getTermComm() + "')";
        q.exec(str);
    }
}

QSqlDatabase MyDataBase::getBase() const {
    return base;
}
