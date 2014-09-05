#ifndef UDPCOMMANDS_H
#define UDPCOMMANDS_H
#include <QUdpSocket>
#include <QGlobalStatic>

#define XMOS_IPADRESS "192.168.2.99"
#define LOCAL_IPADRESS "127.0.0.1"
#define RX_IPADRESS "127.0.0.1"
#define LOCAL_PORT 2222
#define XMOS_PORT 2222

#define DATAGRAM_RX_MAXLEN 1024
#define WRITEDATAGRAM UDP_Socket->writeDatagram(datagram.data(), datagram.size(), *IP_XMOS,  XMOS_PORT);

struct Network{
    QUdpSocket  *UDP_Socket;
    QHostAddress *IP_XMOS;
    QHostAddress *IP_LOCAL;
    QHostAddress *IP_RX;
    quint16 *port_XMOS;
};

enum UDPcommands{
                     MUTE_ALL,
                     MASTERVOLUME,
                     EFFECT,
                     INPUT_SOURCE,
                     PROGRAM_CHANGED,
                     SYNC_TO_XMOS,
                     SYNC_FROM_XMOS,
                     PING,
                     MUTE_CHANNEL,
                     INVERT_DAC_CHANNEL,
                     DAC_OUTPUTGAIN,
                     CHANNEL_CHANGED,
                     DELAY_CHANGED,
                     EQ_ACTIVE_CHANGED,
                     FILTERTYPE_CHANGED,
                     Q_CHANGED,
                     FC_CHANGED,
                     GAIN_CHANGED,
                     CROSSOVER_FREQ,
                     PNK_CHANGED,
                     GET_DACsettings,
                     GET_EQsettings,
                     GET_MIXERsettings
            };


#endif // UDPCOMMANDS_H
