#ifndef MLINKUDP_H
#define MLINKUDP_H

#include <QObject>
#include <QUdpSocket>
#include <mavlink.h>

class mLinkUdp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int udpRxPort MEMBER _udpRxPort NOTIFY udpRxPortChanged)
    Q_PROPERTY(int udpTxPort MEMBER _udpTxPort NOTIFY udpTxPortChanged)
    Q_PROPERTY(bool udpRxState READ udpRxState NOTIFY udpRxStateChanged)
    Q_PROPERTY(bool udpTxState READ udpTxState NOTIFY udpTxStateChanged)

    QUdpSocket *mySocket;
    QByteArray txBuf, rxBuf;
    mavlink_message_t rxMsg;
    mavlink_status_t rxStat;
    int _udpRxPort, _udpTxPort;
    bool _udpRxState = false, _udpTxState = false;

    void initUdp();
    void readMessage();

public:
    explicit mLinkUdp(QObject *parent = nullptr);
    mavlink_message_t rxMessage(){
        return rxMsg;
    }
    void txMessage(mavlink_message_t msg);
    bool udpRxState(){
        return _udpRxState;
    };
    bool udpTxState(){
        return _udpTxState;
    };

signals:
    void udpRxPortChanged();
    void udpTxPortChanged();
    void mLinkMsgAvailable();
    void udpRxStateChanged();
    void udpTxStateChanged();

};

#endif // MLINKUDP_H
