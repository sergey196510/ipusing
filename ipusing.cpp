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

    dbname = "ipusing.db";

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbname);
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
    connect(ui->action_Setting, SIGNAL(triggered(bool)), SLOT(settings()));

    ipl = new IPList(this);
    ui->mainToolBar->addActions(ipl->sact);
    setCentralWidget(ipl);
}

void IPUsing::settings(QWidget *parent)
{
    Settings *s = new Settings(this, dbname);

    s->exec();
    qDebug() << s->DbName() << "\n";

    delete s;
}

IPUsing::~IPUsing()
{
    QSettings settings("IST", "IPUsing");
    settings.setValue("size", size());

    delete ui;
    db.close();
}
