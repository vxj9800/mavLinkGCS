#include "mydrone.h"

myDrone::myDrone(QObject *parent) : QObject(parent)
{
    ps4.setProperty("loc","/dev/input/js0");
    connect(&ps4,&controller::inputReady,this,&myDrone::readController);
    connect(&comms,&mLinkUdp::mLinkMsgAvailable,this,&myDrone::procMLinkMsg);
    connect(&hBeatRx,&QTimer::timeout,this,&myDrone::hBeatRxTimeout);
    connect(&hBeatTx,&QTimer::timeout,this,&myDrone::hBeatTxTimeout);
    hBeatRx.start(3000); hBeatTx.start(1000);
    setconnectorStat("Waiting For connection...");
}

void myDrone::procMLinkMsg()
{
    mavlink_message_t msg = comms.rxMessage();
    switch(msg.msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
    {
        _hBeat = true; emit hBeatChanged();
        hBeatRx.stop(); hBeatRx.start(3000);
        setconnectorStat("Heartbeat received!");
        break;
    }
    case MAVLINK_MSG_ID_COMMAND_ACK:
    {
        mavlink_command_ack_t command;
        mavlink_msg_command_ack_decode(&msg,&command);
        if (command.result == MAV_RESULT::MAV_RESULT_ACCEPTED)
            qDebug() << "Arming acknowledgement receieved.";
        else
            qDebug() << "Arming had error code " << command.result;
        break;
    }
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
    {
        mavlink_global_position_int_t gPos;
        mavlink_msg_global_position_int_decode(&msg,&gPos);
        _heading = gPos.hdg; emit headingChanged();
        qDebug() << _heading;
        break;
    }
    }
}
