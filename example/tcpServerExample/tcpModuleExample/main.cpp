#include "TcpServrExample.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpServrExample w;
    w.show();
    return a.exec();
}
