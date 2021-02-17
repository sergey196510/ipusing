#include "editfreenetwork.h"
#include "ui_editfreenetwork.h"

editFreeNetwork::editFreeNetwork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editFreeNetwork)
{
    ui->setupUi(this);

    connect(ui->networkEdit, SIGNAL(editingFinished()), SLOT(checkFreeNetwork()));
}

editFreeNetwork::~editFreeNetwork()
{
    delete ui;
}

string editFreeNetwork::Network()
{
    return ui->networkEdit->text().toStdString();
}

QString editFreeNetwork::Description()
{
    return ui->descrEdit->text();
}

void editFreeNetwork::setNetwork(const QString &value)
{
    ui->networkEdit->setText(value);
}

void editFreeNetwork::checkFreeNetwork()
{
    QSqlQuery q;

    if (ui->networkEdit->text().isEmpty())
        return;

    IPNetwork a = ui->networkEdit->text().toStdString();
//    auto aa     = a.Network();
//    auto ab     = a.Broadcast();

    q.prepare("SELECT addr FROM ipaddr WHERE parent = 0");
    if (!q.exec()) {
        return;
    }
    while (q.next()) {
        IPNetwork b = q.value(0).toString().toStdString();
        if (!a.Crossing(b)) {
//        auto ba     = b.Network();
//        auto bb     = b.Broadcast();
//        if (!(ab < ba || aa > bb)) {
            QMessageBox::critical(this,
                                  "Пересечение сетей",
                                  QString("Введенная сеть %1 пересекается с сетью %2").arg(a.toString().c_str()).arg(b.toString().c_str()));
            return;
        }
    }
}
