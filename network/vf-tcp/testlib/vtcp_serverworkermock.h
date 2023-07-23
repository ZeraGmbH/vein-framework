#ifndef VEIN_TCP_SERVERWORKERMOCK_H
#define VEIN_TCP_SERVERWORKERMOCK_H

#include "vtcp_server.h"
#include "vtcp_peer.h"
#include "vtcp_serverworkerinterface.h"
#include <QList>

namespace VeinTcp
{
class TcpServerWorkerMock : public QObject, public TcpServerWorkerInterface
{
    Q_OBJECT
private:
    // Just our friend TcpWorkerFactoryMethodsTest can create us (by make_unique) - see
    // https://devblogs.microsoft.com/oldnewthing/20220721-00/?p=106879
    struct secret { explicit secret() = default; };
    friend class TcpWorkerFactoryMethodsTest;
public:
    TcpServerWorkerMock(TcpServer *server, secret);
    bool startServer(quint16 port, bool systemdSocket) override;
    bool isListenActive() override;
private:
    bool m_bListening = false;
    QList<TcpPeer*> m_clients;
    TcpServer *m_server = nullptr;
};
}
#endif // VEIN_TCP_SERVERWORKERMOCK_H
