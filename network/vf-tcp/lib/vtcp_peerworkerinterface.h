#ifndef VTCP_PEERWORKERINTERFACE_H
#define VTCP_PEERWORKERINTERFACE_H

#include <QObject>
#include <QUuid>
#include <memory>

namespace VeinTcp
{
class TcpPeerWorkerInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~TcpPeerWorkerInterface() = default;
    virtual void startConnection(QString ipAddress, quint16 port) = 0;

    // for the sake of resouremanager
    virtual QString getIpAddress() const = 0;
    virtual quint16 getPort() const = 0;
    // for zenux-services
    virtual bool isConnected() const = 0;
    virtual void writeRaw(QByteArray message) const = 0;

    virtual QUuid getPeerId() const  = 0;
    virtual void setPeerId(QUuid peerId) = 0;

    virtual void sendArray(const QByteArray &byteArray) const = 0;
};

typedef std::unique_ptr<TcpPeerWorkerInterface> TcpPeerWorkerInterfacePtr;
}

#endif // VTCP_PEERWORKERINTERFACE_H
