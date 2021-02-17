#ifndef IPUSING_H
#define IPUSING_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QSettings>
#include "iplist.h"
#include "ipnetwork.h"
#include "editfreenetwork.h"

namespace Ui {
class IPUsing;
}

class IPUsing : public QMainWindow
{
    Q_OBJECT

public:
    explicit IPUsing(QWidget *parent = nullptr);
    ~IPUsing();

private:
    QSqlDatabase db;
    Ui::IPUsing *ui;
    IPList *ipl;
};

#endif // IPUSING_H
