#include "networkdebuggingassistant.h"
#include "testForm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    networkDebuggingAssistant w;
    w.show();
    return a.exec();
}
