#ifndef TCPSERVERTEST_H
#define TCPSERVERTEST_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include "../../common/network/tcpSocketHead.h"

using namespace TCP_SocketPublicInfo;
using namespace TCP_ServerInfo;

class tcpServerTest : public QTcpServer
{
    Q_OBJECT
public:
    explicit tcpServerTest(QObject *parent = Q_NULLPTR);
    ~tcpServerTest();

    //连接释放信号与槽相关
    void init_connect();
    void serverDisconnect();
    void init_clientSocketConnect(QTcpSocket* clientSocket);
    void clientSocketDisconnect(QTcpSocket* clientSocket);

    //成员函数
    QList<QTcpSocket*> *getClientList();
    socket_info* getHostInfo(QTcpSocket* socket);
    void deleteHostInfo(socket_info* info);
    QMap<QTcpSocket*, socket_info*>* getSockInfoMap();

    bool createServer(const QHostAddress& hostAddr, quint16 port);
    int ServerSendDataToClient(QTcpSocket* clientSocket, QByteArray& data);

    static QList<QHostAddress> getLocalAddrList();
    static QList<QNetworkInterface> getAllLocalNetworkInterface();
/*************************信号与槽*************************/
public slots:

    /**
     * @brief           由新连接时触发，在此槽函数中获取 QTcpSocket 与目标进行通信。
     *          将获取到的 QTcpSocket 添加进QList<QTcpSocket*> clientList 中。
     *          通过 QTcpSocket 获取 Socket_info ,将Socket_info添加进sockInfoMap中。
     *          成功获取后调用 sendSocketConnect()向上层类发送新客户端连接成功信号。
     * @author          wq
     * @time            2020-12-02
     * @param
     * @return
     * @remarks
     */
    void onNewConnection();//QTcpServer 的 newConnection() 信号
    //与socket连接
    void onSocketConnection();

    /**
     * @brief           客户端断开处理槽函数，此函数负责删除断开客户端信息
     * @author          wq
     * @time            2020-12-02
     * @param
     * @return
     * @remarks
     */
    void onSocketDisconnection();

    /**
     * @brief           Socket状态改变处理槽函数，暂未使用
     * @author          wq
     * @time            2020-12-02
     * @param
     * @return
     * @remarks
     */
    void onSocketStatChanged(QAbstractSocket::SocketState);

    /**
     * @brief           套接字出错槽函数，暂未实现
     * @author          wq
     * @time            2020-12-02
     * @param
     * @return
     * @remarks
     */
    void onSocketError(QAbstractSocket :: SocketError socketError);

    /**
     * @brief           有数据可读则触发此槽函数进行数据的读取
     *          并将数据传递给上层类
     * @author          wq
     * @time            2020-12-02
     * @param
     * @return
     * @remarks
     */
    void onSocketReadyRead();

    /**
     * @brief           关闭服务器，由上层类发送信号触发，一般是上层UI类的按键触发
     *          循环断开所有QTcpSocket信号与槽，关闭所有客户端，删除所有客户端信息
     * @author          wq
     * @time            2020-12-02
     * @param           int connectType:    连接类型
     * @return
     * @remarks
     */
    void closeServerSlot(int connectType);//关闭服务器,释放相关资源

signals:
    //发送客户端连接信号
    void socketConnect(QTcpSocket*);
    //发送客户端断开信号
    void socketDisconnect(QTcpSocket*);
    //收到数据信号
    void socketRevDataToClientSignal(QTcpSocket* socket, QByteArray* data);


     /**
     * @brief           此信号是主动关闭服务器信号,发送函数在上层类中调用
     * @author          wq
     * @time            2020-12-03
     * @param           int connectType 连接类型
     * @return
     * @remarks
     */
    //在上层类调用
    void closeServerSignal(int connectType);

public://发送信号内联函数
    inline void sendSocketConnect(QTcpSocket* socket)
    {
        emit socketConnect(socket);
    }
    inline void sendSocketDisconnect(QTcpSocket* socket)
    {
        emit socketDisconnect(socket);
    }
    inline void sendSocketRevDataToClientSignal(QTcpSocket* socket,
                                                QByteArray* data)
    {
        emit socketRevDataToClientSignal(socket, data);
    }

    //由上层类发送本类接收
 /**
 * @brief           信号函数,当上层类希望主动关闭服务器的时候发送此信号
 * @author          wq
 * @time            2020-12-03
 * @param
 * @return
 * @remarks
 */
    inline void sendCloseServer(int connectType){emit closeServerSignal(connectType);};
/*************************变量区***************************/
public:
    server_info* serverInfo = Q_NULLPTR;//用于创建服务器
    QTcpSocket* currentClient;
    //QList<QTcpSocket*> clientList;//连接上来的客户端列表
private:
    //私有变量
    QList<QTcpSocket*> clientList;//连接上来的客户端列表
    QMap<QTcpSocket*, socket_info*> sockInfoMap;//socket与socket_info关联，socker_info 是对方主机信息
    quint16 socketStat = QAbstractSocket::UnconnectedState;//socket状态,暂未使用
};

#endif // TCPSERVERTEST_H
