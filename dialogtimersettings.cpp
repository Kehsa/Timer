#include "dialogtimersettings.h"
#include "ui_dialogtimersettings.h"

DialogTimerSettings::DialogTimerSettings(MyTimerKeeper *t, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTimerSettings) {
    ui->setupUi(this);
    tasktimer = t;
    ui->ediortName->setText( t->getName() );
    if (t->getMode() == MyTimerKeeper::MODE_ALARMCLOCK)
        ui->editalarmtime->setTime(t->getTargetTimeQTime());
    else if (t->getMode() == MyTimerKeeper::MODE_TIMER) {
        ui->tabWidget->setCurrentIndex(1);
        ui->edittimerhour->setValue(t->getConfTimeH());
        ui->edittimermin->setValue(t->getConfTimeM());
    }
    this->setWindowTitle(tasktimer->getName());
    connect( tasktimer, &MyTimerKeeper::nameisChange, this, &QWidget::setWindowTitle );
    ui->textBrowser->setText(t->getTermComm());
}

DialogTimerSettings::~DialogTimerSettings() {
    delete ui;
}

void DialogTimerSettings::on_btnOK_clicked() {
    tasktimer->setName(ui->ediortName->text());
    tasktimer->stop();
    if (ui->tabWidget->currentIndex() == 0) {
        tasktimer->setMode(MyTimerKeeper::MODE_ALARMCLOCK);
        tasktimer->setConfTime(ui->editalarmtime->time());
    } else {
        tasktimer->setMode(MyTimerKeeper::MODE_TIMER);
        tasktimer->setConfTime(ui->edittimerhour->value(), ui->edittimermin->value());
    }
    tasktimer->setTermComm(QString(ui->textBrowser->text()));
    tasktimer->drop();
    this->close();
}

void DialogTimerSettings::on_btnNo_clicked() {
    this->close();
}
