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
    QUuid getPeerId() const { return m_peerId; };
    void setPeerId(QUuid peerId) {
        Q_ASSERT(!peerId.isNull());
        m_peerId = peerId;
    }
    virtual void sendArray(const QByteArray &byteArray) const = 0;
private:
    QUuid m_peerId;
};

typedef std::unique_ptr<TcpPeerWorkerInterface> TcpPeerWorkerInterfacePtr;

}

#endif // VTCP_PEERWORKERINTERFACE_H
