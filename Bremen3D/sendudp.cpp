#include "sendudp.h"
#include "UDPcommands.h"

UDP::UDP()
{
    udpSocket = new QUdpSocket;

    XMOSPort=2222;
    XMOSIP.setAddress(QString(XMOS_IPADRESS));
}

void UDP::send(QByteArray *datagram){
        udpSocket->writeDatagram(datagram->data(), datagram->size() , XMOSIP , XMOSPort);
}


