#ifndef MYDRONE_H
#define MYDRONE_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QVariant>
#include <QtGlobal>
#include <QTimer>
#include <unistd.h>
#include <mavlink.h>
#include <mlinkudp.h>
#include <controller.h>

class myDrone : public QObject
{
    Q_OBJECT

//    Class properties
    Q_PROPERTY(QString connectionStat READ connectionStat NOTIFY connectionStatChanged)
    Q_PROPERTY(int rxPort MEMBER _rxPort WRITE set_rxPort)
    Q_PROPERTY(int txPort MEMBER _txPort WRITE set_txPort)
    Q_PROPERTY(bool hBeat MEMBER _hBeat NOTIFY hBeatChanged)
    Q_PROPERTY(quint16 heading READ heading NOTIFY headingChanged)

protected:
//    Private data
    mLinkUdp comms;
    int _rxPort, _txPort;
    QString _connectionStat = "";
    QTimer hBeatRx, hBeatTx;
    bool _hBeat = 0;
    controller ps4;
    quint16 _heading = UINT16_MAX;

//    Private Methods


public:
    explicit myDrone(QObject *parent = nullptr);

//    Public Data

//    Class Methods
    QString connectionStat(){
        return _connectionStat;
    };

    void setconnectorStat(QString str){
        _connectionStat = str; emit connectionStatChanged();
    };

    void set_rxPort(int port){
        comms.setProperty("udpRxPort",port); _rxPort = port;
        if (comms.udpRxState() == true)
            setconnectorStat("UDP Transmission Connection Established");
        else
            setconnectorStat("UDP Transmission Connection ERROR!");
    };

    void set_txPort(int port){
        comms.setProperty("udpTxPort",port); _txPort = port;
    }

    Q_INVOKABLE void armDrone()
    {
        mavlink_message_t msg;
        mavlink_msg_command_long_pack(255,1,&msg,1,1,MAV_CMD_COMPONENT_ARM_DISARM,0,1,0,NAN,NAN,NAN,NAN,NAN);
        comms.txMessage(msg);
    }

    Q_INVOKABLE void manualDrone()
    {
        mavlink_message_t msg;
        mavlink_msg_command_long_pack(255,1,&msg,1,1,MAV_CMD_DO_SET_MODE,0,MAV_MODE_MANUAL_ARMED,NAN,NAN,NAN,NAN,NAN,NAN);
        comms.txMessage(msg);
    }

    quint16 heading()
    {
        return _heading;
    }

signals:
    void connectionStatChanged();
    void hBeatChanged();
    void headingChanged();

public slots:
    void procMLinkMsg();
    void hBeatRxTimeout(){
        _hBeat = false; emit hBeatChanged();
        setconnectorStat("Heartbeat not received!");
    };
    void hBeatTxTimeout(){
        if (_hBeat)
        {
            mavlink_message_t sendMsg;
            mavlink_msg_heartbeat_pack(255,1,&sendMsg,MAV_TYPE_GCS,MAV_AUTOPILOT_INVALID,MAV_MODE_MANUAL_ARMED,0,MAV_STATE_ACTIVE);
            comms.txMessage(sendMsg);
        }
    }
    void readController(){
        if (_hBeat)
        {
            mavlink_message_t sendMsg;
            mavlink_msg_manual_control_pack(255,1,&sendMsg,1,-ps4.pitch*1000,ps4.roll*1000,ps4.throtl*1000,ps4.yaw*1000,0);
            comms.txMessage(sendMsg);
        }
    }
};

#endif // MYDRONE_H
