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
    virtual ~TcpPeerPrivate();
    void startConnection(QString ipAddress, quint16 port);
    bool isConnected() const;
    QString getErrorString() const;
    void sendArray(const QByteArray &byteArray) const;

private slots:
    void onReadyRead();
    void closeConnection();
private:
    QByteArray readArray() const;
    QTcpSocket *m_tcpSock = nullptr;
    TcpPeer *q_ptr = nullptr;
};
}
#endif // VEIN_TCP_PEERPRIVATE_H
