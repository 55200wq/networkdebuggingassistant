#include "networkdebuggingassistant.h"
#include "ui_networkdebuggingassistant.h"

#include <QColorDialog>

networkDebuggingAssistant::networkDebuggingAssistant(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::networkDebuggingAssistant)
{
    ui->setupUi(this);
    customContextMenu = new QMenu;
    customContextMenu->addAction(ui->action_SetWidthBgColor);
    customContextMenu->addAction(ui->action_setFont);
    server = new tcpServerTest(this);
}

networkDebuggingAssistant::~networkDebuggingAssistant()
{
    delete ui;
    delete customContextMenu;
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

#include <QDebug>
void networkDebuggingAssistant::on_centralwidget_customContextMenuRequested(const QPoint &pos)
{

    customContextMenu->show();
    customContextMenu->move(cursor().pos());//移动到width当前位置
}


void networkDebuggingAssistant::on_action_SetWidthBgColor_triggered()
{
    QPalette palet = ui->centralwidget ->palette();
    QColor inicolor; //= palet.color(QPalette::Background);
    QColor color = QColorDialog::getColor(inicolor, this, "选择颜色");
    if(color.isValid()){
        palet.setColor(QPalette::Background, color);
        ui->centralwidget->setPalette(palet);
    }
}

#include <QMessageBox>
void networkDebuggingAssistant::on_action_setFont_triggered()
{
    QMessageBox box;
    box.setWindowTitle("ggg");
    box.setText("暂不支持");
    box.open();
    box.exec();
}

/***************** network 相关 ****************/
void networkDebuggingAssistant::socketConnectSlot(QTcpSocket* client)
{

}
void networkDebuggingAssistant::socketDisconnectSlot(QTcpSocket* client)
{

}
void networkDebuggingAssistant::socketRevDataToClientSlot(QTcpSocket* client, QByteArray* data)
{

}
