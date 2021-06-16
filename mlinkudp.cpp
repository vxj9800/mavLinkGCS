#include "mlinkudp.h"

mLinkUdp::mLinkUdp(QObject *parent) : QObject(parent)
{
    mySocket = new QUdpSocket();
    connect(mySocket,&QUdpSocket::readyRead,this,&mLinkUdp::readMessage);
    connect(this,&mLinkUdp::udpRxPortChanged,this,&mLinkUdp::initUdp);
}

void mLinkUdp::readMessage()
{
    rxBuf.resize(mySocket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    mySocket->readDatagram(rxBuf.data(), rxBuf.size(), &sender, &senderPort);
    if (sender == QHostAddress::LocalHost && senderPort == _udpTxPort)
    {
        _udpTxState = true;
        for (int i=0; i<rxBuf.size();i++)
        {
            if (mavlink_parse_char(MAVLINK_COMM_0,rxBuf.at(i),&rxMsg,&rxStat))
            {
//                qDebug()<<"Received packet: SYS:"<< mLinkMsg.sysid
//                       <<"COMP:"<<mLinkMsg.compid
//                      <<"LEN:"<<mLinkMsg.len
//                     <<"MSG ID:"<<mLinkMsg.msgid;
            }
        }
        emit mLinkMsgAvailable();
    }
    else
        _udpTxState = false;
}

void mLinkUdp::txMessage(mavlink_message_t msg)
{
    uint8_t sendBuff[MAVLINK_MAX_PACKET_LEN];
    int len = mavlink_msg_to_send_buffer(sendBuff, &msg);
    mySocket->writeDatagram((char *)sendBuff,len,QHostAddress::LocalHost,_udpTxPort);
}

void mLinkUdp::initUdp()
{
    mySocket->close();
    if (mySocket->bind(QHostAddress::LocalHost, _udpRxPort,QAbstractSocket::ShareAddress))
        _udpRxState = true;
    else
        _udpRxState = false;
}
