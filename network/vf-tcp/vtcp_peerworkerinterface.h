#ifndef VTCP_PEERWORKERINTERFACE_H
#define VTCP_PEERWORKERINTERFACE_H

#include <QObject>
#include <memory>

namespace VeinTcp
{
class TcpPeerWorkerInterface : public QObject
{
    Q_OBJECT
public:
    virtual void startConnection(QString ipAddress, quint16 port) = 0;
    virtual QString getErrorString() const = 0;
    virtual void sendArray(const QByteArray &byteArray) const = 0;
};

typedef std::unique_ptr<TcpPeerWorkerInterface> TcpPeerWorkerInterfacePtr;

}

#endif // VTCP_PEERWORKERINTERFACE_H
