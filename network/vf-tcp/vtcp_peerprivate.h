#ifndef VEIN_TCP_PEERPRIVATE_H
#define VEIN_TCP_PEERPRIVATE_H

#include <QTcpSocket>

namespace VeinTcp
{
class TcpPeer;

class TcpPeerPrivate
{
private:
    TcpPeerPrivate(TcpPeer *publicPeer);
    TcpPeerPrivate(TcpPeer *publicPeer, qintptr socketDescriptor);
    bool isConnected() const;
    QByteArray readArray() const;
    void sendArray(const QByteArray &byteArray) const;

    QTcpSocket *m_tcpSock = nullptr;
    TcpPeer *q_ptr = nullptr;

    friend class TcpPeer;
};
}
#endif // VEIN_TCP_PEERPRIVATE_H
