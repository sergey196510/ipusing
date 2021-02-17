#ifndef EDITFREENETWORK_H
#define EDITFREENETWORK_H

#include <string>
#include <QObject>
#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include "ipnetwork.h"

using namespace std;

namespace Ui {
class editFreeNetwork;
}

class editFreeNetwork : public QDialog
{
    Q_OBJECT

public:
    explicit editFreeNetwork(QWidget *parent = nullptr);
    ~editFreeNetwork();
    void setNetwork(const QString &value);
    string Network();
    QString Description();

private:
    Ui::editFreeNetwork *ui;

private slots:
    void checkFreeNetwork();
};

#endif // EDITFREENETWORK_H
