#pragma once

#include <QFile>
#include <QTextStream>
#include <QKeyEvent>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QStyle>

#include "mytimerkeeper.h"
#include "dialogtimersettings.h"
#include "mydatabase.h"

namespace Ui {
    class Timer;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    std::vector<MyTimerKeeper*> timerList;
    MyDataBase *timersDataBase;
    DialogTimerSettings *timerSettingsDialog = NULL;

    bool isControlPressed = false;
    QSystemTrayIcon tray;
    QAction *max;
    QAction *min;
    QAction *out;
    Ui::Timer *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    virtual void keyReleaseEvent(QKeyEvent *) override;

private slots:
    virtual void keyPressEvent(QKeyEvent *) override;
    void trayClick(QSystemTrayIcon::ActivationReason);
    void About();
    void on_btnNew_clicked();
    void on_btnDel_clicked();
    void on_btnAllEnable_clicked();
    void on_btnAllDisable_clicked();
    void callTimerEditDialog(MyTimerKeeper*);
    void closeTimerSettingsDialog();
    void newTimer(const QString name, const QString termCommands, int h, int m, int mode);

protected:
    void closeEvent(QCloseEvent *);
};
