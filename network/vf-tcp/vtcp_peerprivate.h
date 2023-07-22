#ifndef VEIN_TCP_PEERPRIVATE_H
#define VEIN_TCP_PEERPRIVATE_H

#include <QTcpSocket>

namespace VeinTcp
{
class TcpPeer;

class TcpPeerPrivate
{
private:
    TcpPeerPrivate(TcpPeer *t_publicPeer);
    TcpPeerPrivate(TcpPeer *t_publicPeer, qintptr t_socketDescriptor);
    bool isConnected() const;
    QByteArray readArray() const;
    void sendArray(const QByteArray &t_byteArray) const;

    QTcpSocket *m_tcpSock = nullptr;
    TcpPeer *q_ptr = nullptr;

    friend class TcpPeer;
};
}
#endif // VEIN_TCP_PEERPRIVATE_H
