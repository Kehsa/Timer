#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Timer) {
    const QString TIMER_DATABASE = QString(qApp->applicationDirPath() +
           #ifdef Q_OS_LINUX
               "/" +
           #endif
           #ifdef Q_OS_WIN
               "\\" +
           #endif
            "TimerSQLiteDataBase");
    ui->setupUi(this);
    QTableWidget *w = ui->tableWidget;
    w->setColumnCount(6);
    w->setHorizontalHeaderItem(0, new QTableWidgetItem("Название"));
    w->setHorizontalHeaderItem(1, new QTableWidgetItem("Сработает"));
    w->setHorizontalHeaderItem(2, new QTableWidgetItem("Включен"));
    w->setHorizontalHeaderItem(3, new QTableWidgetItem("Время"));
    w->setHorizontalHeaderItem(4, new QTableWidgetItem("Сброс"));
    w->setHorizontalHeaderItem(5, new QTableWidgetItem("Настройки"));

    max = new QAction("Восстановить", this);
    min = new QAction("Свернуть в трей", this);
    out = new QAction("Выход", this);
    QMenu *m = new QMenu(this);
    m->addAction(max);
    m->addAction(min);
    m->addAction(out);

    tray.setContextMenu(m);
    tray.setIcon( QIcon(":/images/timer"));

    connect( this->max, &QAction::triggered, this, &QWidget::show );
    connect( this->min, &QAction::triggered, this, &QWidget::hide );
    connect( this->out, SIGNAL(triggered()), qApp, SLOT(quit()) );
    connect( this->ui->MenuOut, &QAction::triggered, qApp, &QApplication::quit );
    connect( this->ui->MenuTray, &QAction::triggered, this, &MainWindow::hide );
    connect( this->ui->menuAbout, &QAction::triggered, this, &MainWindow::About);
    connect( &this->tray, &QSystemTrayIcon::activated, this, &MainWindow::trayClick );

    this->setWindowIcon( QIcon(":/images/timer"));
    timersDataBase = new MyDataBase(TIMER_DATABASE);

    auto ls = timersDataBase->getTimers();
    for (auto &x : ls) {
        QString name, com;
        int h, m, mode;
        std::tie(name, com, h, m, mode) = x;
        newTimer(name, com, h, m, mode);
    }
    this->show();
    tray.show();
}

MainWindow::~MainWindow() {
    timersDataBase->saveTimers(timerList);
    delete max;
    delete min;
    delete out;
    delete ui;
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Control) {
        isControlPressed = false;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Control:
        isControlPressed = true;
        break;
    case Qt::Key_Q:
        QApplication::quit();
    }
}

void MainWindow::trayClick(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        if (this->isHidden()) {
            this->show();
        } else {
            this->hide();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *) {
    this->hide();
}

void MainWindow::About() {
    QFile file(QString(":/About/FAQ"));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString str;
        str = stream.readAll();
        QMessageBox *messageWindow = new QMessageBox( QMessageBox::NoIcon, QString("О программе"),
                                                      str, QMessageBox::NoButton, this, Qt::Dialog);
        messageWindow->setModal(true);
        messageWindow->show();
    }
}

void MainWindow::on_btnNew_clicked() {
    newTimer("Some Timer", "", 6, 0, 0);
}

void MainWindow::newTimer(const QString name, const QString termCommands, int h, int m, int mode) {
    auto *tmp = new MyTimerKeeper(name, termCommands, h, m, mode, this);

    QTableWidget *w = ui->tableWidget;
    w->setRowCount(w->rowCount() + 1);
    int row = w->rowCount() - 1;

    QLabel *tmplabel = new QLabel(QString(tmp->getName()), this);
    connect( tmp, &MyTimerKeeper::nameisChange, tmplabel, &QLabel::setText );
    w->setCellWidget(row, 0, tmplabel);

    tmplabel = new QLabel("     ", this);
    connect( tmp, &MyTimerKeeper::timerTick, tmplabel, &QLabel::setText );
    w->setCellWidget(row, 1, tmplabel);

    QWidget *tmpWidget = new QWidget();
    QCheckBox *tmpchbox = new QCheckBox(this);
    QHBoxLayout *tmpLayout = new QHBoxLayout(tmpWidget);
    tmpLayout->addWidget(tmpchbox);
    tmpLayout->setAlignment(Qt::AlignCenter);
    tmpLayout->setContentsMargins(0,0,0,0);
    tmpWidget->setLayout(tmpLayout);
    tmpchbox->setChecked(tmp->isEnable());
    connect( tmpchbox, &QCheckBox::stateChanged, tmp, &MyTimerKeeper::setTimerState );
    connect( tmp, &MyTimerKeeper::startStop, tmpchbox, &QAbstractButton::setChecked );
    w->setCellWidget(row, 2, tmpWidget);

    tmplabel = new QLabel(QString(tmp->getName()), this);
    connect( tmp, &MyTimerKeeper::timeIsChange, tmplabel, &QLabel::setText );
    w->setCellWidget(row, 3, tmplabel);

    QPushButton *btn = new QPushButton( style()->standardIcon(QStyle::SP_BrowserReload), "", this);
    connect( btn, &QAbstractButton::clicked, tmp, &MyTimerKeeper::drop );
    w->setCellWidget(row, 4, btn);

    btn = new QPushButton("Настроить", this);
    connect( btn, &QAbstractButton::clicked, tmp, &MyTimerKeeper::edit );
    w->setCellWidget(row, 5, btn);

    connect( tmp, &MyTimerKeeper::wantLoadEditDialog, this, &MainWindow::callTimerEditDialog );
    tmp->timeRequest();
    timerList.push_back(tmp);
}

void MainWindow::on_btnDel_clicked() {
    int row = ui->tableWidget->currentRow();
    if (row == -1) {
        return;
    }
    ui->tableWidget->removeRow(row);
    MyTimerKeeper *x = timerList.at(row);
    auto iter = timerList.begin();
    std::advance(iter, row);
    timerList.erase(iter);
    delete x;
}

void MainWindow::on_btnAllEnable_clicked() {
    if (!timerList.empty())
        for (MyTimerKeeper* x : timerList)
            x->start();
}

void MainWindow::on_btnAllDisable_clicked() {
    if (!timerList.empty())
        for (MyTimerKeeper* x : timerList)
            x->stop();
}

void MainWindow::callTimerEditDialog(MyTimerKeeper *timer) {
    if (timerSettingsDialog) {
        timerSettingsDialog->setFocus();
    } else {
        DialogTimerSettings *dial;
        dial = new DialogTimerSettings(timer, this);
        connect( timer, &QObject::destroyed, dial, &QWidget::close );
        connect( dial, &QDialog::finished, this, &MainWindow::closeTimerSettingsDialog );
        timerSettingsDialog = dial;
        dial->show();
    }
}

void MainWindow::closeTimerSettingsDialog() {
    timerSettingsDialog = NULL;
}
