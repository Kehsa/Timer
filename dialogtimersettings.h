#pragma once

#include <vector>

#include <QtWidgets/QDialog>

#include "mytimerkeeper.h"

namespace Ui {
    class DialogTimerSettings;
}

class DialogTimerSettings : public QDialog {
    Q_OBJECT
private:
    MyTimerKeeper *tasktimer;
public:
    explicit DialogTimerSettings(MyTimerKeeper *t, QWidget *parent = 0);
    ~DialogTimerSettings();

private slots:
    void on_btnOK_clicked();
    void on_btnNo_clicked();

private:
    Ui::DialogTimerSettings *ui;
};
