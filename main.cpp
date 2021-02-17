#include "ipusing.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IPUsing w;
    w.show();

    return a.exec();
}
