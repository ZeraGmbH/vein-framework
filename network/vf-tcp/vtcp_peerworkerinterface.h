#ifndef VTCP_PEERWORKERINTERFACE_H
#define VTCP_PEERWORKERINTERFACE_H

#include <QObject>

namespace VeinTcp
{
class TcpPeerWorkerInterface : public QObject
{
    Q_OBJECT
public:
    virtual void startConnection(QString ipAddress, quint16 port) = 0;
    virtual bool isConnected() const = 0;
    virtual QString getErrorString() const = 0;
    virtual void sendArray(const QByteArray &byteArray) const = 0;
};

}

#endif // VTCP_PEERWORKERINTERFACE_H
