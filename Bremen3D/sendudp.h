#ifndef SENDUDP_H
#define SENDUDP_H
#include <QUdpSocket>

class UDP
{
public:
    explicit UDP();
    void send(QByteArray *datagram);

private:
    quint16 XMOSPort;
    QHostAddress XMOSIP;
    QByteArray *datagram;
    QUdpSocket *udpSocket;

};

#endif // SENDUDP_H
