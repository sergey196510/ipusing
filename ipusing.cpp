#include <QFile>
#include "ipusing.h"
#include "ui_ipusing.h"

IPUsing::IPUsing(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IPUsing)
{
    bool createdb = false;

    ui->setupUi(this);

    QSettings settings("IST", "IPUsing");
    resize(settings.value("size", QSize(400,400)).toSize());

    if (!QFile("ipusing.db").exists()) {
        int ret = QMessageBox::warning(0,
                             "Database",
                             "Database is not exist.\nCreate database?",
                             QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
            createdb = true;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ipusing.db");
    if (!db.open()) {
        qDebug() << db.lastError();
        exit(1);
    }

    if (createdb == true) {
        QSqlQuery q;
        q.prepare("create table ipaddr(id integer primary key autoincrement, parent integer default 0, busy integer default 0, addr text, descr text)");
        q.exec();
    }

    connect(ui->menuHelp, SIGNAL(triggered(QAction*)), qApp, SLOT(aboutQt()));
    connect(ui->action_Quit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

    ipl = new IPList(this);
    ui->mainToolBar->addActions(ipl->sact);
    setCentralWidget(ipl);
}

IPUsing::~IPUsing()
{
    QSettings settings("IST", "IPUsing");
    settings.setValue("size", size());

    delete ui;
    db.close();
}
