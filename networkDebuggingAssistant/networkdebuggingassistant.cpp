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

