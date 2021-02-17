#ifndef EDITSIZENETWORK_H
#define EDITSIZENETWORK_H

#include <QDialog>

namespace Ui {
class editSizeNetwork;
}

class editSizeNetwork : public QDialog
{
    Q_OBJECT

public:
    explicit editSizeNetwork(QWidget *parent = nullptr);
    ~editSizeNetwork();
    int Value();

private:
    Ui::editSizeNetwork *ui;
};

#endif // EDITSIZENETWORK_H
