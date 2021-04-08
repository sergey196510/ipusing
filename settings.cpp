#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent, QString dbname) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->lineEdit->setText(dbname);
}

Settings::~Settings()
{
    delete ui;
}

QString Settings::DbName()
{
    return ui->lineEdit->text();
}
