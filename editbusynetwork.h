#ifndef EDITBUSYNETWORK_H
#define EDITBUSYNETWORK_H

#include <QDialog>
#include "ipnetwork.h"

namespace Ui {
class editBusyNetwork;
}

class editBusyNetwork : public QDialog
{
    Q_OBJECT

public:
    explicit editBusyNetwork(QWidget *parent = nullptr);
    ~editBusyNetwork();
    void setNetwork(const QString &str);
    void setDescription(const QString &str);
    QString Network();
    QString Description();
    void hideNetwork();

public slots:
    void checkAddress();

private:
    Ui::editBusyNetwork *ui;
};

#endif // EDITBUSYNETWORK_H
