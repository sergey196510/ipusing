#include "editbusynetwork.h"
#include "ui_editbusynetwork.h"

editBusyNetwork::editBusyNetwork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editBusyNetwork)
{
    ui->setupUi(this);

    connect(ui->networkEdit, SIGNAL(editingFinished()), SLOT(checkAddress()));
}

editBusyNetwork::~editBusyNetwork()
{
    delete ui;
}

void editBusyNetwork::setNetwork(const QString &str)
{
    ui->networkEdit->setText(str);
}

void editBusyNetwork::setNetwork(IPNetwork &str)
{
    ui->networkEdit->setText(str.toString().c_str());
}

void editBusyNetwork::setDescription(const QString &str)
{
    ui->descrEdit->setText(str);
}

QString editBusyNetwork::Network()
{
    return ui->networkEdit->text();
}

QString editBusyNetwork::Description()
{
    return ui->descrEdit->text();
}

void editBusyNetwork::hideNetwork()
{
    ui->networkEdit->setEnabled(false);
}

void editBusyNetwork::checkAddress()
{
    qDebug() << "check address:" << ui->networkEdit->text();
}
