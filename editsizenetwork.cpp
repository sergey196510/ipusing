#include "editsizenetwork.h"
#include "ui_editsizenetwork.h"

editSizeNetwork::editSizeNetwork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editSizeNetwork)
{
    ui->setupUi(this);

    ui->sizeBox->setMinimum(0);
    ui->sizeBox->setMaximum(32);
    ui->sizeBox->setValue(24);
}

editSizeNetwork::~editSizeNetwork()
{
    delete ui;
}

int editSizeNetwork::Value()
{
    return ui->sizeBox->value();
}
