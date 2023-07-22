#ifndef VEIN_TCP_PEERPRIVATE_H
#define VEIN_TCP_PEERPRIVATE_H

#include <QUuid>
#include <QTcpSocket>

namespace VeinTcp
{
class TcpPeer;

class TcpPeerPrivate
{
private:
    explicit TcpPeerPrivate(TcpPeer *t_publicPeer);
    QByteArray readArray() const;
    void sendArray(const QByteArray &t_byteArray) const;

    QUuid m_peerId;
    QTcpSocket *m_tcpSock = nullptr;
    TcpPeer *q_ptr = nullptr;

    friend class TcpPeer;
};
}
#endif // VEIN_TCP_PEERPRIVATE_H
