#include "clientview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientView w;
    w.show();

    return a.exec();
}
