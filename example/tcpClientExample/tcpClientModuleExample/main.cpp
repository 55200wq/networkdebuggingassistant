#include "tcpClientModuleExample.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tcpClientModuleExample w;
    w.show();
    return a.exec();
}
