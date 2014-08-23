#include "sendudp.h"

UDP::UDP()
{
    udpSocket = new QUdpSocket;

    XMOSPort=2222;
    XMOSIP.setAddress(QString("192.168.2.19"));
}

void UDP::send(QByteArray *datagram){
        udpSocket->writeDatagram(datagram->data(), datagram->size(),XMOSIP, XMOSPort);
}


