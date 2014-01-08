#ifndef UDPCOMMANDS_H
#define UDPCOMMANDS_H
#include <QUdpSocket>
#include <QGlobalStatic>

#define XMOS_IPADRESS "127.0.0.1"
#define LOCAL_IPADRESS "127.0.0.1"
#define RX_IPADRESS "127.0.0.1"
#define LOCAL_PORT 2222
#define XMOS_PORT 2223

#define DATAGRAM_RX_MAXLEN 1024
#define WRITEDATAGRAM UDP_Socket->writeDatagram(datagram.data(), datagram.size(), *IP_XMOS,  XMOS_PORT);

struct Network{
    QUdpSocket  *UDP_Socket;
    QHostAddress *IP_XMOS;
    QHostAddress *IP_LOCAL;
    QHostAddress *IP_RX;
    quint16 *port_XMOS;
};

enum UDPcommands{MUTE_ALL,
                 MASTERVOLUME,
                 INPUT_SOURCE,
                 PROGRAM_CHANGED,
                 SYNC_TO_XMOS,
                 SYNC_FROM_XMOS,
                 PING,
                 CHANNEL_CHANGED,
                 DELAY_CHANGED,
                 EQ_CHANGED,
                 Q_CHANGED,
                 FC_CHANGED,
                 GAIN_CHANGED,
                 CROSSOVER_FREQ,
                 MIXERVALUE
                };





#endif // UDPCOMMANDS_H
