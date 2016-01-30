#pragma once

#include <QObject>
#include <QtCore>
#include <QThread>

class MyTimerKeeper : public QObject {
    Q_OBJECT
public:
    static const int MODE_ALARMCLOCK = 0;
    static const int MODE_TIMER = 1;

private:
    static const int TIMERSTEP_SECOND = 1000;
    static const int TIMERSTEP_MINUTE = 60000;
    int _timerStep = 1000;

    QTimer _timer;
    QString _termComm;
    QString _name;
    int _targetTimeHours;
    int _targetTimeMin;
    int _confTimeHours;
    int _confTimeMin;
    int _mode;

public:
    explicit MyTimerKeeper(const QString name, const QString termCommands, int h, int m, int mode, QObject *parent = 0);

    bool isEnable();
    QString getName();
    int getMode();
    int getConfTimeH();
    int getConfTimeM();
    QTime getTargetTimeQTime();
    const QString getTermComm();

    void setMode(int m);
    void setName(const QString &newName);
    void setTermComm(const QString &taskLs);
    void setConfTime(const QTime &t);
    void setConfTime(int h, int m);
    void setTimerState(int newState);
    void stop();
    void start();
    void reStart();
    void drop();
    void changeTimerInterval(int);
    void edit();
    void timeRequest();

private:
    void timertick();
    void end();
    void setTimerStep(int);

signals:
    void timeIsChange(QString);
    void timerStepChanget(int);
    void timerTick(QString);
    void nameisChange(QString);
    void timerEnd();
    void modeisChange(int);
    void startStop(bool);
    void wantLoadEditDialog(MyTimerKeeper*);
};
