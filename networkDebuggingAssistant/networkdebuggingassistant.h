#ifndef NETWORKDEBUGGINGASSISTANT_H
#define NETWORKDEBUGGINGASSISTANT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class networkDebuggingAssistant; }
QT_END_NAMESPACE

typedef  enum{
    TCP_SERVER,
    TCP_CLIENT,
    UDP
} ConnectType;

class networkDebuggingAssistant : public QMainWindow
{
    Q_OBJECT

public:
    networkDebuggingAssistant(QWidget *parent = nullptr);
    ~networkDebuggingAssistant();

private slots:
    void on_cBx_connectType_activated(int index);

private:
    Ui::networkDebuggingAssistant *ui;
};
#endif // NETWORKDEBUGGINGASSISTANT_H
