#include "networkdebuggingassistant.h"
#include "../src/camera/caremaCollection.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    caremaCollection cam;
    //networkDebuggingAssistant w;
    //w.show();
    return a.exec();
}
