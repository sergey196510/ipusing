#ifndef IPUSING_H
#define IPUSING_H

#include <QObject>
#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QSettings>
#include "iplist.h"
#include "ipnetwork.h"
#include "settings.h"

namespace Ui {
class IPUsing;
}

class IPUsing : public QMainWindow
{
    Q_OBJECT

public:
    explicit IPUsing(QWidget *parent = nullptr);
    ~IPUsing();

public slots:
    void settings(QWidget *parent = nullptr);

private:
    QSqlDatabase db;
    Ui::IPUsing *ui;
    IPList *ipl;
    Settings *st;
    QString dbname;
};

#endif // IPUSING_H
