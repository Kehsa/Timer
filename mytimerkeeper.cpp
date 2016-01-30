#include "mytimerkeeper.h"

MyTimerKeeper::MyTimerKeeper(const QString name, const QString termCommands, int h, int m, int mode, QObject *parent) :
    QObject(parent), _name(name), _termComm(termCommands) {
    _mode = mode;
    _targetTimeHours = _confTimeHours = h;
    _targetTimeMin = _confTimeMin = m;
    connect( this, &MyTimerKeeper::timerStepChanget, this, &MyTimerKeeper::changeTimerInterval );
    connect( &_timer, &QTimer::timeout, this, &MyTimerKeeper::timertick );
}

bool MyTimerKeeper::isEnable() {
    return _timer.isActive();
}

QString MyTimerKeeper::getName() {
    return _name;
}

int MyTimerKeeper::getMode() {
    return _mode;
}

int MyTimerKeeper::getConfTimeH() {
    return _confTimeHours;
}

int MyTimerKeeper::getConfTimeM() {
    return _confTimeMin;
}

QTime MyTimerKeeper::getTargetTimeQTime() {
    QTime t(_targetTimeHours, _targetTimeMin);
    return t;
}

void MyTimerKeeper::setMode(int m) {
    if (m == 0 || m == 1) {
        _mode = m;
        emit modeisChange(m);
    } else {
        qWarning() << "ERROR: void TimerKeeper::setMode(int m)\n Bad mode m;" << endl;
    }
}

void MyTimerKeeper::setName(const QString &newName) {
    _name = newName;
    emit nameisChange(_name);
}

void MyTimerKeeper::setTermComm(const QString &newComm) {
     _termComm = newComm;
}

void MyTimerKeeper::setConfTime(const QTime &t) {
    this->setConfTime(t.hour(), t.minute());
}

void MyTimerKeeper::setConfTime(int h, int m) {
    _confTimeHours = h;
    _confTimeMin = m;
    this->timeRequest();
}

void MyTimerKeeper::setTimerState(int newState) {
    if (newState) {
        start();
    } else {
        stop();
    }
}

void MyTimerKeeper::stop() {
    _timer.stop();
    emit startStop(false);
}

void MyTimerKeeper::start() {
    if (_mode == MyTimerKeeper::MODE_TIMER) {
        _targetTimeMin += _targetTimeHours * 60;
        _targetTimeHours = 0;
    }
    setTimerStep(TIMERSTEP_SECOND);
    _timer.start();
    emit startStop(true);
}

void MyTimerKeeper::reStart() {
    this->drop();
    this->start();
}

void MyTimerKeeper::drop() {
    _targetTimeHours = _confTimeHours;
    _targetTimeMin = _confTimeMin;
    this->stop();
}

void MyTimerKeeper::changeTimerInterval(int x) {
    _timer.setInterval(x);
}

void MyTimerKeeper::edit() {
    emit wantLoadEditDialog(this);
}

void MyTimerKeeper::timeRequest() {
    QString s;
    if (_mode == MyTimerKeeper::MODE_TIMER) {
        s = QString(" через ") + QString::number(_confTimeHours) + QString("ч. ")
                + QString::number(_confTimeMin) + QString("м.");
    } else {
        s = QString("    в ") + QString::number(_confTimeHours) + QString("ч. ") + QString::number(_confTimeMin) + QString("м.");
    }
    emit timeIsChange(s);
}

const QString MyTimerKeeper::getTermComm() {
    return _termComm;
}

void MyTimerKeeper::timertick() {
    if (_timerStep == TIMERSTEP_SECOND) {
        int cursec = QTime::currentTime().second();
        if (cursec >= 0 && cursec < 2)
            setTimerStep(TIMERSTEP_MINUTE);
    }
    if (_mode == MyTimerKeeper::MODE_TIMER) {
        if (_timerStep == TIMERSTEP_MINUTE) {
            _targetTimeMin--;
        }
        if (_targetTimeMin == 0) {
            end();
            return;
        }
        int m = _targetTimeMin;
        int h = m / 60;
        m -= h * 60;
        emit timerTick(QString(" через ") + QString().setNum(h) + QString("ч. ") + QString().setNum(m) + QString("м."));
    }
    if (_mode == MyTimerKeeper::MODE_ALARMCLOCK) {
        QTime curent( QTime::currentTime());
        QTime targetime(_targetTimeHours, _targetTimeMin);
        if (curent.hour() == targetime.hour()
             && curent.minute() == targetime.minute() ) {
         end();
         return;
        }
        emit timerTick(QString("    в ") + QString().setNum(_targetTimeHours) + QString("ч. ") + QString().setNum(_targetTimeMin) + QString("м."));
    }
}

void MyTimerKeeper::end() {
    stop();
    system(_termComm.toUtf8().data());
    emit timerEnd();
}

void MyTimerKeeper::setTimerStep(int x) {
    _timerStep = x;
    emit timerStepChanget(x);
}
