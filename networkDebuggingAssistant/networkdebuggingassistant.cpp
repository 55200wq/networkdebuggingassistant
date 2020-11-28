#include "networkdebuggingassistant.h"
#include "ui_networkdebuggingassistant.h"

networkDebuggingAssistant::networkDebuggingAssistant(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::networkDebuggingAssistant)
{
    ui->setupUi(this);
}

networkDebuggingAssistant::~networkDebuggingAssistant()
{
    delete ui;
}



void networkDebuggingAssistant::on_cBx_connectType_activated(int index)
{
    switch (index) {
    case TCP_SERVER:
        break;
    case TCP_CLIENT:
        break;
    case UDP:
        break;
    default:
        break;
    }
}
