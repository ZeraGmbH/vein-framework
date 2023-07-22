#ifndef VEIN_TCP_PEERPRIVATE_H
#define VEIN_TCP_PEERPRIVATE_H

#include <QTcpSocket>

namespace VeinTcp
{
class TcpPeer;

class TcpPeerPrivate : public QObject
{
    Q_OBJECT
public:
    TcpPeerPrivate(TcpPeer *publicPeer);
    TcpPeerPrivate(TcpPeer *publicPeer, qintptr socketDescriptor);
    void startConnection(QString ipAddress, quint16 port);
    bool isConnected() const;

private slots:
    void onReadyRead();
private:
    QByteArray readArray() const;
    void sendArray(const QByteArray &byteArray) const;

    QTcpSocket *m_tcpSock = nullptr;
    TcpPeer *q_ptr = nullptr;

    friend class TcpPeer;
};
}
#endif // VEIN_TCP_PEERPRIVATE_H
